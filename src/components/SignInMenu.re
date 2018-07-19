exception AuthenticationFailure(array(string));

let component = ReasonReact.statelessComponent("SignInMenu");

let str = ReasonReact.string;

module GetAuthenticationSaltQuery = [%graphql
  {|
  query($email: String!) {
    user(email: $email) {
      authenticationSalt
    }
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
              team {
                name
              }
              invitingUser {
                email
              }
            }
          }
        }
        errors
      }
    }
  |}
];

let handleSubmit = (appSend, event) => {
  event |> DomUtils.preventEventDefault;
  let email =
    DomUtils.getValueOfInputById("sign-in-form__email") |> Email.create;
  let password = DomUtils.getValueOfInputById("sign-in-form__password");
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
       let salt = response##user##authenticationSalt |> Salt.fromString;
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
              Js.Promise.resolve((rawSession, encryptionHash))
            );
       | None => Js.Promise.reject(AuthenticationFailure(response##errors))
       };
     })
  |> Js.Promise.then_(((rawSession, encryptionHash)) => {
       let accessToken = rawSession##token |> AccessToken.create;
       let session = Session.create(accessToken, encryptionHash);
       let key = session |> Session.getCryptographicKey;
       Team.decryptTeams(
         (rawSession, session),
         key,
         rawSession##user##teams,
       );
     })
  |> Js.Promise.then_((((rawSession, session), teams)) => {
       session |> Session.saveInLocalStorage;
       appSend(
         Turaku.SignIn(
           session,
           teams,
           rawSession##user##incomingInvitations
           |> Array.map(i =>
                InvitationFromTeam.create(
                  i##id,
                  ~teamName=i##team##name,
                  ~invitingUserEmail=i##invitingUser##email |> Email.create,
                )
              )
           |> Array.to_list,
         ),
       );
       Js.Promise.resolve();
     })
  |> ignore;
};

let signedUpAlert = (data: Turaku.signInPageData) =>
  if (data.justSignedUp) {
    <div className="p-2 bg-yellow-light rounded mb-3">
      (
        str(
          "Thank you for signing up! Please confirm your email address before signing in.",
        )
      )
    </div>;
  } else {
    ReasonReact.null;
  };

let gotoSignUp = (appSend, _event) => Turaku.SelectSignUp |> appSend;

let make = (~data, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div className="container mx-auto px-4">
      <div className="flex justify-center h-screen">
        <div className="w-full md:w-1/2 self-auto md:self-center pt-4 md:pt-0">
          (signedUpAlert(data))
          <form onSubmit=(handleSubmit(appSend))>
            <div>
              <label htmlFor="sign-in-form__email">
                (str("Email address"))
              </label>
              <input
                autoFocus=true
                required=true
                _type="email"
                className="rounded bg-grey-light focus:bg-grey-lighter p-2 mt-2 w-full"
                id="sign-in-form__email"
              />
            </div>
            <div className="mt-3">
              <label htmlFor="sign-in-form__password">
                (str("Password"))
              </label>
              <input
                required=true
                _type="password"
                className="rounded bg-grey-light focus:bg-grey-lighter p-2 mt-2 w-full"
                id="sign-in-form__password"
              />
            </div>
            <button _type="submit" className="mt-5 btn btn-blue">
              (str("Sign In"))
            </button>
            <a
              onClick=(gotoSignUp(appSend))
              className="mt-5 ml-2 btn border hover:bg-grey-light cursor-pointer">
              (str("Sign Up"))
            </a>
          </form>
        </div>
      </div>
    </div>,
};