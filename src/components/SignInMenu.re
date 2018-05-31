let component = ReasonReact.statelessComponent("SignInMenu");

let str = ReasonReact.stringToElement;

type createSessionResponse = {
  id: int,
  token: string,
  user: User.t,
  teams: list(Team.t),
  invitations: list(Invitation.t),
};

module GetAuthenticationSaltConfig = [%graphql
  {|
  query($email: String!) {
    user(email: $email) {
      authenticationSalt
    }
  }
  |}
];

module SignInConfig = [%graphql
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
                id
                name
              }
              invitingUser {
                email
              }
            }
          }
        }
      }
    }
  |}
];

module Codec = {
  let encodeEmail = email =>
    Json.Encode.object_([("email", email |> Json.Encode.string)]);
  let encodeEmailAndPassword = (~email, ~password) =>
    Json.Encode.object_([
      (
        "session",
        Json.Encode.object_([
          ("email", email |> Json.Encode.string),
          ("password", password |> Json.Encode.string),
        ]),
      ),
    ]);
  let decodeAuthenticationSalt = json =>
    json |> Json.Decode.field("salt", Json.Decode.string);
  let decodeSignInResponse = json =>
    Json.Decode.{
      id: json |> field("id", int),
      token: json |> field("token", string),
      user: json |> field("user", User.decode),
      teams: json |> field("teams", list(Team.decode)),
      invitations:
        json |> field("incoming_invitations", list(Invitation.decode)),
    };
};

module Service = {
  let loadAuthenticationSalt = (~email) => {
    let apiRequest =
      ApiRequest.create(~purpose=ApiRequest.GetAuthenticationSalt);
    let params = email |> Codec.encodeEmail;
    ApiRequest.fetch(~apiRequest, ~params)
    |> Js.Promise.then_(response =>
         response |> Codec.decodeAuthenticationSalt |> Js.Promise.resolve
       );
  };
  let signInWithHashedPassword = (~email, ~password, ~authenticationSalt) =>
    HashUtils.hexHash(password, ~salt=authenticationSalt, ())
    |> Js.Promise.then_(hash => {
         let apiRequest = ApiRequest.create(~purpose=ApiRequest.SignIn);
         let params = Codec.encodeEmailAndPassword(~email, ~password=hash);
         ApiRequest.fetch(~apiRequest, ~params);
       })
    |> Js.Promise.then_(encodedResponse =>
         encodedResponse |> Codec.decodeSignInResponse |> Js.Promise.resolve
       );
  /* Save the session in storage to allow it to be restored without signing in again on a page reload. */
  let saveSession = (~token, ~encryptionHash) => {
    Dom.Storage.setItem("token", token, Dom.Storage.sessionStorage);
    Dom.Storage.setItem(
      "encryptionHash",
      encryptionHash,
      Dom.Storage.sessionStorage,
    );
  };
  let signIn = (email: string, password: string) =>
    loadAuthenticationSalt(~email)
    |> Js.Promise.then_(authenticationSalt =>
         signInWithHashedPassword(~email, ~password, ~authenticationSalt)
       )
    |> Js.Promise.then_(decodedResponse => {
         let encryptionSalt = decodedResponse.user |> User.encryptionSalt;
         HashUtils.hexHash(password, ~salt=encryptionSalt, ())
         |> Js.Promise.then_(encryptionHash =>
              Js.Promise.resolve((decodedResponse, encryptionHash))
            );
       })
    |> Js.Promise.then_(responseAndHash => {
         let (decodedResponse, encryptionHash) = responseAndHash;
         let token = decodedResponse.token;
         saveSession(~token, ~encryptionHash);
         Js.Promise.resolve(responseAndHash);
       });
};

let handleSubmit = (appSend, event) => {
  event |> DomUtils.preventEventDefault;
  let email = DomUtils.getValueOfInputById("sign-in-form__email");
  let password = DomUtils.getValueOfInputById("sign-in-form__password");
  Js.log(
    "Calling Service.signIn for " ++ email ++ " with password " ++ password,
  );
  let _ =
    Service.signIn(email, password)
    |> Js.Promise.then_(responseAndHash => {
         let (response, encryptionHash) = responseAndHash;
         appSend(
           Turaku.SignedIn(
             response.token,
             response.teams,
             response.invitations,
             encryptionHash,
           ),
         );
         Js.Promise.resolve();
       });
  ();
};

let signedUpAlert = (appState: Turaku.state) =>
  if (appState.flags.justSignedUp) {
    <div className="alert alert-success" role="alert">
      (
        str(
          "Thank you for signing up! Please confirm your email address before signing in.",
        )
      )
    </div>;
  } else {
    ReasonReact.nullElement;
  };

let gotoSignUp = (appSend, _event) =>
  appSend(Turaku.(Navigate(SignUpPage)));

let make = (~appState, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div className="container">
      <div className="row justify-content-center sign-in__centered-container">
        <div className="col-md-6 align-self-center">
          (signedUpAlert(appState))
          <form onSubmit=(handleSubmit(appSend))>
            <div className="form-group">
              <label htmlFor="sign-in-form__email">
                (str("Email address"))
              </label>
              <input
                required=(Js.Boolean.to_js_boolean(true))
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
                required=(Js.Boolean.to_js_boolean(true))
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
/* export default class SignIn extends React.Component {
     componentWillMount() {
       if (this.props.appState.justSignedUp) {
         this.props.setAppState({ justSignedUp: false, signedUp: true });
       }
     }

     submit(event) {
       event.preventDefault();
       let email = document.getElementById("sign-in-email").value;
       let password = document.getElementById("sign-in-password").value;
       let signInService = new SignInService(email, password);

       signInService
         .signIn()
         .then(authorization => {
           console.log("Signed in.");
           this.props.setAppState(
             {
               token: authorization.token,
               teams: authorization.teams,
               incomingInvitations: authorization.incomingInvitations,
               encryptionHash: authorization.encryptionHash
             },
             () => {
               this.setState({ signInComplete: true });
             }
           );
         })
         .catch(exception => {
           // Handle invalid credentials / exception.
           console.log(exception, "Sign in failed.");
         });
     }

     justSignedUp() {
       return (
         this.props.appState.redirectFrom === "SignUp" &&
         this.props.appState.redirectTo === "SignIn"
       );
     }

     render() {
       if (this.state.signInComplete === true) {
         return <Redirect to="/teams" />;
       }

       return (

       );
     }
   } */