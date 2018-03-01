type state = {signInComplete: bool};

type action =
  | CompleteSignIn;

let signIn = ReasonReact.reducerComponent("SignIn");

let str = ReasonReact.stringToElement;

let justSignedUp = () => true;

let submit = _event => Js.log("Handle submit.");

let signedUpAlert = () =>
  if (justSignedUp()) {
    <div className="alert alert-success" role="alert">
      (
        str(
          "Thank you for signing up! Please confirm your email address before signing in."
        )
      )
    </div>;
  } else {
    ReasonReact.nullElement;
  };

let make = (~appState, ~appSend, _children) => {
  ...signIn,
  initialState: () => {signInComplete: false},
  reducer: (action, _state) =>
    switch action {
    | CompleteSignIn => ReasonReact.Update({signInComplete: true})
    },
  render: ({state, send}) =>
    <div className="container">
      <div className="row justify-content-center sign-in__centered-container">
        <div className="col-md-6 align-self-center">
          (signedUpAlert())
          <form onSubmit=submit>
            <div className="form-group">
              <label htmlFor="sign-in-email"> (str("Email address")) </label>
              <input
                required=(Js.Boolean.to_js_boolean(true))
                _type="email"
                className="form-control"
                id="sign-in-email"
                placeholder="Your registered email address"
              />
            </div>
            <div className="form-group">
              <label htmlFor="sign-in-password"> (str("Password")) </label>
              <input
                required=(Js.Boolean.to_js_boolean(true))
                _type="password"
                className="form-control"
                id="sign-in-password"
                placeholder="Password"
              />
            </div>
            <button _type="submit" className="mt-2 btn btn-primary">
              (str("Sign In"))
            </button>
            <button className="mt-2 ml-2 btn btn-secondary">
              (str("Sign Up"))
            </button>
          </form>
        </div>
      </div>
    </div>
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