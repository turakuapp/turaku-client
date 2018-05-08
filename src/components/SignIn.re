let signIn = ReasonReact.statelessComponent("SignIn");

let str = ReasonReact.stringToElement;

type response = int;

module Codec = {
  let encodeEmail = email =>
    Json.Encode.object_([("email", email |> Json.Encode.string)]);
  let encodeEmailAndPassword = (~email, ~password) =>
    Json.Encode.object_([
      ("email", email |> Json.Encode.string),
      ("password", password |> Json.Encode.string),
    ]);
};

module Service = {
  let loadAuthenticationSalt = (~email) => {
    let apiRequest =
      ApiRequest.create(~purpose=ApiRequest.GetAuthenticationSalt);
    let params = email |> Codec.encodeEmail;
    ApiRequest.fetch(~apiRequest, ~params);
  };
  let signInWithHashedPassword = (~email, ~password, ~authenticationSalt) =>
    HashUtils.hexHash(password, ~salt=authenticationSalt, ())
    |> Js.Promise.then_(hash => {
         let apiRequest = ApiRequest.create(~purpose=ApiRequest.SignIn);
         Codec.encodeEmailAndPassword(~email, ~password)
         |> ApiRequest.fetch(~apiRequest);
       });
  /* signInWithHashedPassword(authenticationSalt) {
       let api = new ApiService();

       let hashService = new HashService(this.password, authenticationSalt);

       return hashService.hexHash().then(hash => {
         return api
           .post("sessions", {
             session: {
               email: this.email,
               password: hash
             }
           })
           .then(response => {
             console.log(response, "POST sessions -> success");
             return Promise.resolve({
               token: response.token,
               encryptionSalt: response.user.encryption_salt,
               teams: response.teams,
               incomingInvitations: response.incoming_invitations
             });
           })
           .catch(response => {
             console.log(response, "POST sessions -> failure");
             // TODO: What should be returned if sign in fails?
             return Promise.reject(
               new Error("Response from API indicated a failure.")
             );
           });
       });
     } */
  let signIn = (email: string, password: string) => {
    let a: response = 1;
    Js.Promise.resolve(a);
    loadAuthenticationSalt(~email)
    |> Js.Promise.then_(authenticationSalt =>
         signInWithHashedPassword(~email, ~password, ~authenticationSalt)
       )
    |> Js.Promise.then_(authenticationResponse => {
         decodedResponse = SignIn.Codec.decode(authenticationResponse);
         let encryptionHash =
           createEncryptionHash(
             ~password,
             ~encryptionSalt=decodedResponse.encryptionSalt,
             decodedResponse,
             encryptionHash,
           )
           |> Js.Promise.resolve;
         ();
       })
    |> Js.Promise.then_(responseAndHash => {
         let (decodedResponse, encryptionHash) = responseAndHash;
         saveSession(~decodedResponse, ~encryptionHash);
         Js.Promise.resolve(responseAndHash);
       });
  };
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
    |> Js.Promise.then_((_response: response) => {
         appSend(Turaku.SignedIn);
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
  ...signIn,
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