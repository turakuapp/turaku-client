exception AuthenticationFailure(array(string));

let component = ReasonReact.statelessComponent("SignInMenu");

let str = ReasonReact.string;

type createSessionResponse = {
  id: int,
  token: string,
  user: User.t,
  teams: list(Team.t),
  invitations: list(Invitation.t),
};

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
  |> Js.Promise.then_(rawSessionAndHash => {
       let (rawSession, encryptionHash) = rawSessionAndHash;
       let accessToken = rawSession##token |> AccessToken.create;
       let session = Session.create(accessToken, encryptionHash);
       let key = session |> Session.getCryptographicKey;
       Team.decryptTeams(
         (rawSession, session),
         key,
         rawSession##user##teams,
       );
     })
  |> Js.Promise.then_(sessionsAndTeams => {
       let ((rawSession, session), teams) = sessionsAndTeams;
       session |> Session.saveInLocalStorage;
       appSend(
         Turaku.SignIn(
           session,
           teams,
           rawSession##user##incomingInvitations
           |> Array.map(i =>
                Invitation.create(
                  i##id,
                  ~teamName=i##team##name,
                  ~userEmail=i##invitingUser##email |> Email.create,
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
    <div className="alert alert-success" role="alert">
      (
        str(
          "Thank you for signing up! Please confirm your email address before signing in.",
        )
      )
    </div>;
  } else {
    ReasonReact.null;
  };

let gotoSignUp = (appSend, _event) =>
  appSend(Turaku.(Navigate(Turaku.SignedOutUser(SignUpPage))));

let make = (~data, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div className="container">
      <div className="row justify-content-center sign-in__centered-container">
        <div className="col-md-6 align-self-center">
          (signedUpAlert(data))
          <form onSubmit=(handleSubmit(appSend))>
            <div className="form-group">
              <label htmlFor="sign-in-form__email">
                (str("Email address"))
              </label>
              <input
                required=true
                _type="email"
                className="form-control"
                id="sign-in-form__email"
                placeholder="Your registered email address"
              />
            </div>
            <div className="form-group">
              <label htmlFor="sign-in-form__password">
                (str("Password"))
              </label>
              <input
                required=true
                _type="password"
                className="form-control"
                id="sign-in-form__password"
                placeholder="Password"
              />
            </div>
            <button _type="submit" className="mt-2 btn btn-primary">
              (str("Sign In"))
            </button>
            <button
              onClick=(gotoSignUp(appSend))
              className="mt-2 ml-2 btn btn-secondary">
              (str("Sign Up"))
            </button>
          </form>
        </div>
      </div>
    </div>,
};