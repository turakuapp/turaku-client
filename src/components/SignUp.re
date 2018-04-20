let str = ReasonReact.stringToElement;

type state = {signUpComplete: bool};

type action =
  | CompleteSignUp;

let signUp = ReasonReact.reducerComponent("SignUp");

module Encode = {
  let user = (name, email, password, authenticationSalt) =>
    Json.Encode.object_([
      ("name", name |> Json.Encode.string),
      ("email", email |> Json.Encode.string),
      ("password", password |> Json.Encode.string),
      ("authentication_salt", authenticationSalt |> Json.Encode.string),
    ]);
  let request = (name, email, password, authenticationSalt) =>
    Json.Encode.object_([
      ("user", user(name, email, password, authenticationSalt)),
    ]);
};

module Service = {
  let saltPossibleCharacters = "1234567890abcdef";
  /* TODO: This uses a potentially insecure randomization method. Use cryptographically secure alternative in production. */
  let authenticationSalt = length => {
    let rec aux = (salt, remainingLength) =>
      if (remainingLength > 0) {
        let randomPosition =
          Random.int(String.length(saltPossibleCharacters));
        let saltCharacter =
          Js.String.charAt(randomPosition, saltPossibleCharacters);
        aux(salt ++ saltCharacter, remainingLength - 1);
      } else {
        salt;
      };
    aux("", length);
  };
  let signUp = (name, email, password) => {
    let salt = authenticationSalt(64);
    HashUtils.hexHash(password, ~salt, ())
    |> Js.Promise.then_(hexHash => {
         let apiRequest: ApiRequest.t = {
           token: None,
           baseUrl: ApiRequest.DefaultBaseUrl,
         };
         Js.log("Created a apiRequest.");
         apiRequest
         |> ApiRequest.post(
              ApiRequest.SignUp,
              Encode.request(name, email, hexHash, salt),
            )
         |> Js.Promise.then_(response => {
              Js.log(response);
              Js.Promise.resolve();
            });
         Js.Promise.resolve();
       });
    /* signUpService
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
       }); */
    /* let api = new ApiService();
       let salt = this.authenticationSalt();
       let hashService = new HashService(password, salt);

       return hashService.hexHash().then(hash => {
         return api
           .post("users", {
             user: {
               name: name,
               email: email,
               password: hash,
               authentication_salt: salt
             }
           })
           .then(response => {
             console.log(response, "success");
           })
           .catch(response => {
             console.log(response, "failure");
             // TODO: What should be returned if sign up fails?
             return Promise.reject(
               new Error("Response from API indicated a failure.")
             );
           });
       }); */
  };
};

let handleSubmit = event => {
  event |> DomUtils.preventEventDefault;
  let name = DomUtils.getValueOfInputById("sign-up-form__name");
  let email = DomUtils.getValueOfInputById("sign-up-form__email");
  let password = DomUtils.getValueOfInputById("sign-up-form__password");
  Js.log(
    "Calling Service.signUp for "
    ++ name
    ++ " ("
    ++ email
    ++ ") with password "
    ++ password,
  );
  Service.signUp(name, email, password);
  ();
};

let make = (~appState, ~appSend, _children) => {
  ...signUp,
  initialState: () => {signUpComplete: false},
  reducer: (action, _state) =>
    switch (action) {
    | CompleteSignUp => ReasonReact.Update({signUpComplete: true})
    },
  render: _self =>
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
                required=(Js.Boolean.to_js_boolean(true))
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
                required=(Js.Boolean.to_js_boolean(true))
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
                required=(Js.Boolean.to_js_boolean(true))
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
                required=(Js.Boolean.to_js_boolean(true))
              />
              <small id="emailHelp" className="form-text text-muted">
                (
                  str(
                    "Please store this password in your personal password manager. The Turaku team cannot recover your data if you forget your password. ",
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
    </div>,
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