type createSessionError = [
  | `AuthenticationFailure
  | `InvalidEmail
  | `UnconfirmedEmail
];

exception CreateSessionFailure(array(createSessionError));

type error =
  | AuthenticationFailure
  | InvalidEmail
  | UnconfirmedEmail
  | UnexpectedError(string);

type state = {
  errors: list(error),
  email: Email.t,
  password: string,
};

type action =
  | AddError(error)
  | UpdateEmail(string)
  | UpdatePassword(string)
  | ClearErrors;

let str = React.string;

module GetAuthenticationSaltQuery = [%graphql
  {|
  query($email: String!) {
    authenticationSalt(email: $email)
  }
  |}
];

module SignInQuery = [%graphql
  {|
    mutation ($email: String!, $password: String!) {
      createSession(email: $email, password: $password) {
        session {
          token
          user {
            encryptionSalt
          }
        }
        teams {
          id
          name
          encryptedPassword {
            iv
            ciphertext
          }
        }
        incomingInvitations {
          id
          teamName
          invitingUser {
            email
          }
        }
        errors
      }
    }
  |}
];

let handleCreateSessionFailure = send =>
  [@bs.open]
  (
    fun
    | CreateSessionFailure(errors) => {
        errors
        |> Array.iter(error =>
             switch (error) {
             | `AuthenticationFailure =>
               send(AddError(AuthenticationFailure))
             | `InvalidEmail => send(AddError(InvalidEmail))
             | `UnconfirmedEmail => send(AddError(UnconfirmedEmail))
             }
           );
        Js.Promise.resolve();
      }
  );

let handleSubmit = (appSend, state, send, event) => {
  send(ClearErrors);
  event |> DomUtils.preventEventDefault;

  let {email, password} = state;

  Js.log(
    "Attempting to sign in with email "
    ++ (email |> Email.toString)
    ++ " and password "
    ++ password,
  );

  /* Fetch the authentication salt to hash the password before attempting to sign in. */
  GetAuthenticationSaltQuery.make(~email=email |> Email.toString, ())
  |> Api.sendPublicQuery
  |> Js.Promise.then_(response => {
       let salt = response##authenticationSalt |> Salt.fromString;
       AuthenticationHash.create(password, salt);
     })
  |> Js.Promise.then_(authenticationHash =>
       SignInQuery.make(
         ~email=email |> Email.toString,
         ~password=authenticationHash,
         (),
       )
       |> Api.sendPublicQuery
     )
  |> Js.Promise.then_(rawResponse => {
       let response = rawResponse##createSession;
       switch (response##session) {
       | Some(rawSession) =>
         let encryptionSalt = rawSession##user##encryptionSalt;
         EncryptionHash.create(password, encryptionSalt)
         |> Js.Promise.then_(encryptionHash =>
              Js.Promise.resolve((
                rawSession##token,
                response##teams,
                response##incomingInvitations,
                encryptionHash,
              ))
            );
       | None => Js.Promise.reject(CreateSessionFailure(response##errors))
       };
     })
  |> Js.Promise.then_(((token, teams, incomingInvitations, encryptionHash)) => {
       let accessToken = token |> AccessToken.create;
       let session = Session.create(accessToken, encryptionHash);
       let key = session |> Session.getCryptographicKey;
       let teams = Team.decryptTeams(key, teams);

       let invitations =
         incomingInvitations
         |> Array.map(i =>
              InvitationFromTeam.create(
                i##id,
                ~teamName=i##teamName,
                ~invitingUserEmail=i##invitingUser##email |> Email.create,
              )
            )
         |> Array.to_list;

       Js.Promise.all2((Js.Promise.resolve((session, invitations)), teams));
     })
  |> Js.Promise.then_((((session, incomingInvitations), teams)) => {
       session |> Session.saveInLocalStorage;
       appSend(Turaku.SignIn(session, teams, incomingInvitations));
       Js.Promise.resolve();
     })
  |> Js.Promise.catch(error =>
       switch (error |> handleCreateSessionFailure(send)) {
       | Some(x) => x
       | None =>
         Js.log(error);
         send(
           AddError(
             UnexpectedError(
               "Details of the error have been logged to the console.",
             ),
           ),
         );
         Js.Promise.resolve();
       }
     )
  |> ignore;
};

let signedUpAlert = (data: Turaku.signInPageData) =>
  if (data.justSignedUp) {
    <div className="p-2 bg-yellow-light rounded mb-3">
      {
        str(
          "Thank you for signing up! Please confirm your email address before signing in.",
        )
      }
    </div>;
  } else {
    React.null;
  };

let gotoSignUp = (appSend, _event) => Turaku.SelectSignUp |> appSend;

let inputClasses = state => {
  let classes = "rounded p-2 mt-2 w-full";

  if (state.errors != []) {
    classes ++ " text-white bg-red-light focus:bg-red";
  } else {
    classes ++ " bg-grey-light focus:bg-grey-lighter";
  };
};

let toMessage = error =>
  switch (error) {
  | AuthenticationFailure => "These credentials do not match any existing user."
  | InvalidEmail => "This does not look like a valid email address."
  | UnconfirmedEmail => "This is an unconfirmed email address; please visit the confirmation link from your mailbox before trying to log in."
  | UnexpectedError(message) => "Unexpected Error: " ++ message
  };

let toMessageKey = error =>
  switch (error) {
  | AuthenticationFailure => "AuthenticationFailure"
  | InvalidEmail => "InvalidEmail"
  | UnconfirmedEmail => "UnconfirmedEmail"
  | UnexpectedError(_) =>
    "UnexpectedError-" ++ (Js.Math.random_int(100, 999) |> string_of_int)
  };

let errorMessages = state =>
  if (state.errors != []) {
    <ul className="mt-2 list-reset text-sm text-red-darker">
      {
        state.errors
        |> Array.of_list
        |> Array.map(error =>
             <li key={error |> toMessageKey}> {error |> toMessage |> str} </li>
           )
        |> React.array
      }
    </ul>;
  } else {
    React.null;
  };

[@react.component]
let make = (~data, ~appSend) => {
  let (state, send) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | AddError(error) => {...state, errors: [error, ...state.errors]}
        | UpdateEmail(email) => {
            ...state,
            email: email |> Email.create,
            errors: [],
          }
        | UpdatePassword(password) => {...state, password, errors: []}
        | ClearErrors => {...state, errors: []}
        },
      {errors: [], email: Email.create(""), password: ""},
    );

  <div className="container mx-auto px-4">
    <div className="flex justify-center h-screen">
      <div className="w-full md:w-1/2 self-auto md:self-center pt-4 md:pt-0">
        {signedUpAlert(data)}
        <form onSubmit={handleSubmit(appSend, state, send)}>
          <div>
            <label htmlFor="sign-in-form__email">
              {str("Email address")}
            </label>
            <input
              value={state.email |> Email.toString}
              onChange={
                event =>
                  UpdateEmail(event->ReactEvent.Form.target##value) |> send
              }
              autoFocus=true
              required=true
              type_="email"
              className={inputClasses(state)}
            />
          </div>
          {errorMessages(state)}
          <div className="mt-3">
            <label htmlFor="sign-in-form__password">
              {str("Password")}
            </label>
            <input
              value={state.password}
              onChange={
                event =>
                  UpdatePassword(event->ReactEvent.Form.target##value) |> send
              }
              required=true
              type_="password"
              className={inputClasses(state)}
            />
          </div>
          <button type_="submit" className="mt-5 btn btn-blue">
            {str("Sign In")}
          </button>
          <a
            onClick={gotoSignUp(appSend)}
            className="mt-5 ml-2 btn border hover:bg-grey-light cursor-pointer">
            {str("Sign Up")}
          </a>
        </form>
      </div>
    </div>
  </div>;
};