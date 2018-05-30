let str = ReasonReact.stringToElement;

type state = {signUpComplete: bool};

type action =
  | CompleteSignUp;

let signUp = ReasonReact.reducerComponent("SignUpMenu");

/* Create a GraphQL Query by using the graphql_ppx */
module CreateUser = [%graphql
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

module CreateUserMutation = ReasonApollo.CreateMutation(CreateUser);

let gotoSignIn = (appSend, _event) =>
  appSend(Turaku.(Navigate(SignInPage)));

let newCreateUserMutation = () => {
  let name = DomUtils.getValueOfInputById("sign-up-form__name");
  let email = DomUtils.getValueOfInputById("sign-up-form__email");
  let password = DomUtils.getValueOfInputById("sign-up-form__password");
  let authenticationSalt = Salt.create(64);
  HashUtils.hexHash(password, ~salt=authenticationSalt, ())
  |> Js.Promise.then_(hexHash => {
       Js.log("Using CreateUser...");
       CreateUser.make(
         ~name,
         ~email,
         ~password=hexHash,
         ~authenticationSalt,
         (),
       )
       |> Js.Promise.resolve;
     });
};

let handleResult = (appSend, promise) =>
  promise
  |> Js.Promise.then_(data => {
       let parsedData = CreateUserMutation.convertJsInputToReason(data);
       switch (parsedData.data) {
       | None => Js.log("Callback: Some unhandled error occured.")
       | Some(response) =>
         let errors = response##createUser##errors;
         if (errors |> Js.Array.length == 0) {
           appSend(Turaku.SignedUp);
         } else {
           Js.log("Errors: " ++ (errors |> Js.Array.joinWith(", ")));
         };
       };
       Js.Promise.resolve(data);
     });

/* TODO: submitButton() should probably be extracted as another component. */
let submitButton = appSend =>
  <CreateUserMutation>
    ...(
         (mutation, {result}) =>
           <button
             className="mt-2 btn btn-primary"
             onClick=(
               event => {
                 event |> DomUtils.preventMouseEventDefault;
                 newCreateUserMutation()
                 |> Js.Promise.then_(createUserMutation =>
                      mutation(~variables=createUserMutation##variables, ())
                    )
                 |> handleResult(appSend)
                 |> ignore;
               }
             )>
             ("Sign Up" |> str)
             <span>
               (
                 switch (result) {
                 | NotCalled =>
                   Js.log("Not called");
                   "" |> str;
                 | Data(d) =>
                   Js.log2("data", d);
                   "Submission complete" |> str;
                 | Error(e) =>
                   Js.log2("error", e);
                   "ERROR" |> str;
                 | Loading =>
                   Js.log("Loading");
                   "Loading" |> str;
                 }
               )
             </span>
           </button>
       )
  </CreateUserMutation>;

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
          <form>
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
            (submitButton(appSend))
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