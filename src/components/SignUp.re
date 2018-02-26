let str = ReasonReact.stringToElement;

type state = {signUpComplete: bool};

type action =
  | CompleteSignUp;

let signUp = ReasonReact.reducerComponent("SignUp");

let handleSubmit = _event => Js.log("Handle submission, somehow?");

let make = (~appState, ~appSend, _children) => {
  ...signUp,
  initialState: () => {signUpComplete: false},
  reducer: (action, _state) =>
    switch action {
    | CompleteSignUp => ReasonReact.Update({signUpComplete: true})
    },
  render: ({state, send}) =>
    <div className="container">
      <div className="row justify-content-center sign-in__centered-container">
        <div className="col-md-6 align-self-center">
          <form onSubmit=handleSubmit>
            <div className="form-group">
              <label htmlFor="sign-up-form__name"> (str("Name")) </label>
              <input
                className="form-control"
                id="sign-up-form__name"
                placeholder="What should we call you?"
              />
            </div>
            <div className="form-group">
              <label htmlFor="sign-up-form__email">
                (str("Email address"))
              </label>
              <input
                _type="email"
                className="form-control"
                id="sign-up-form__email"
              />
              <small id="emailHelp" className="form-text text-muted">
                (str("We'll never share your email with anyone else."))
              </small>
            </div>
            <div className="form-group">
              <label htmlFor="sign-up-form__password">
                (str("Password"))
              </label>
              <input
                _type="password"
                className="form-control"
                id="sign-up-form__password"
                placeholder="Password"
              />
            </div>
            <div className="form-group">
              <label htmlFor="sign-up-form__password-confirmation">
                (str("Confirm Password"))
              </label>
              <input
                _type="password"
                className="form-control"
                id="sign-up-form__password-confirmation"
                placeholder="Again, to be sure"
              />
              <small id="emailHelp" className="form-text text-muted">
                (
                  str(
                    "Please store this password in your personal password manager. The Turaku team cannot recover your data if you forget your password. "
                  )
                )
                <a href="help.turaku.com/security">
                  (str("Read about Turaku's security"))
                </a>
                (str(" to learn more."))
              </small>
            </div>
            <button _type="submit" className="mt-2 btn btn-primary">
              (str("Submit"))
            </button>
            <button className="mt-2 ml-2 btn btn-secondary">
              (str("Sign In"))
            </button>
          </form>
        </div>
      </div>
    </div>
};
/* export default class SignUp extends React.Component {
     constructor(props) {
       super(props);

       this.state = {
         signUpComplete: false
       };

       this.submit = this.submit.bind(this);
     }

     submit(event) {
       event.preventDefault();

       let name = document.getElementById("sign-up-form__name").value;
       let email = document.getElementById("sign-up-form__email").value;
       let password = document.getElementById("sign-up-form__password").value;

       // TODO: Compare the confirmation against the supplied password.
       // let passwordConfirmation = document.getElementById(
       //   "sign-up-form__password-confirmation"
       // ).value;

       let signUpService = new SignUpService();
       let that = this;

       signUpService
         .signUp(name, email, password)
         .then(() => {
           that.props.setAppState(
             {
               redirectFrom: "SignUp",
               redirectTo: "SignIn"
             },
             () => {
               that.setState({ signUpComplete: true });
             }
           );
         })
         .catch(exception => {
           // Handle exception.
           console.log(exception, "Sign up failed. <shrug>");
         });
     }

     render() {
       if (this.state.signUpComplete === true) {
         return <Redirect to="/sign_in" />;
       }

       return (

       );
     }
   }
    */