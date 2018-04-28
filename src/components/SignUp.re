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
              ApiRequest.SignUpPurpose,
              Encode.request(name, email, hexHash, salt),
            );
       })
    |> Js.Promise.then_(response => {
         Js.log(response);
         Js.Promise.resolve(response);
       });
  };
};

let handleSubmit = (appSend, event) => {
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
  let _ =
    Service.signUp(name, email, password)
    |> Js.Promise.then_((_response: User.t) => {
         appSend(Turaku.SignedUp);
         Js.Promise.resolve();
       });
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
          <form onSubmit=(handleSubmit(appSend))>
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