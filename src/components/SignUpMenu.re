let str = ReasonReact.stringToElement;

type state = {signUpComplete: bool};

type action =
  | CompleteSignUp;

let signUp = ReasonReact.reducerComponent("SignUpMenu");

/* Create a GraphQL Query by using the graphql_ppx */
module CreateUserQuery = [%graphql
  {|
  mutation($name: String!, $email: String!, $authenticationSalt: String!, $password: String!) {
    createUser(name: $name, email: $email, authenticationSalt: $authenticationSalt, password: $password) {
      user {
        id
      }
      errors
    }
  }
|}
];

let gotoSignIn = (appSend, _event) =>
  appSend(Turaku.(Navigate(SignInPage)));

let handleSignUp = ({Turaku.session}, appSend, event) => {
  event |> DomUtils.preventEventDefault;
  let name = DomUtils.getValueOfInputById("sign-up-form__name");
  let email = DomUtils.getValueOfInputById("sign-up-form__email");
  let password = DomUtils.getValueOfInputById("sign-up-form__password");
  let authenticationSalt = Salt.create(64);
  /* Hash the password with the newly generated salt before using it to register an account. */
  HashUtils.hexHash(password, ~salt=authenticationSalt, ())
  |> Js.Promise.then_(hexHash =>
       CreateUserQuery.make(
         ~name,
         ~email,
         ~password=hexHash,
         ~authenticationSalt,
         (),
       )
       |> Api.sendQuery(session)
     )
  |> Js.Promise.then_(response => {
       let errors = response##createUser##errors;
       if (errors |> Js.Array.length == 0) {
         appSend(Turaku.SignUp);
       } else {
         Js.log(Js.Array.joinWith(", ", errors));
       };
       Js.Promise.resolve(response);
     })
  |> ignore;
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
          <form onSubmit=(handleSignUp(appState, appSend))>
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
                    "Please store this password in your personal password manager. If you forget your password, you will need another team admin to invite you back in. Read about ",
                  )
                )
                <a href="help.turaku.com/security">
                  (str("how Turaku secures your data"))
                </a>
                (str(" to learn more."))
              </small>
            </div>
            <button _type="submit" className="mt-2 btn btn-primary">
              (str("Sign Up"))
            </button>
            <button
              onClick=(gotoSignIn(appSend))
              className="mt-2 ml-2 btn btn-secondary">
              (str("Sign In"))
            </button>
          </form>
        </div>
      </div>
    </div>,
};