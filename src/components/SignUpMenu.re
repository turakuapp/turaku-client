let str = React.string;

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

let gotoSignIn = (appSend, _event) => Turaku.SelectSignIn |> appSend;

let handleSignUp = (appSend, setInProgress, event) => {
  event |> DomUtils.preventEventDefault;
  setInProgress(_ => true);
  let name = DomUtils.getValueOfInputById("sign-up-form__name");
  let email = DomUtils.getValueOfInputById("sign-up-form__email");
  let password = DomUtils.getValueOfInputById("sign-up-form__password");
  let authenticationSalt = Salt.create(64);
  /* Hash the password with the newly generated salt before using it to register an account. */
  AuthenticationHash.create(password, authenticationSalt)
  |> Js.Promise.then_(authenticationHash =>
       CreateUserQuery.make(
         ~name,
         ~email,
         ~password=authenticationHash,
         ~authenticationSalt,
         (),
       )
       |> Api.sendPublicQuery
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

[@react.component]
let make = (~appSend) => {
  let (inProgress, setInProgress) = React.useState(() => false);

  <div className="container mx-auto px-4">
    <div className="flex justify-center h-screen">
      <div className="w-full md:w-1/2 self-auto md:self-center pt-4 md:pt-0">
        <form onSubmit={handleSignUp(appSend, setInProgress)}>
          <div className="form-group">
            <label htmlFor="sign-up-form__name"> {str("Name")} </label>
            <input
              className="rounded bg-grey-light focus:bg-grey-lighter p-2 mt-2 w-full"
              id="sign-up-form__name"
              placeholder="What should we call you?"
              required=true
            />
          </div>
          <div className="mt-3">
            <label htmlFor="sign-up-form__email">
              {str("Email address")}
            </label>
            <input
              type_="email"
              className="rounded bg-grey-light focus:bg-grey-lighter p-2 mt-2 w-full"
              id="sign-up-form__email"
              required=true
            />
            <small className="block text-grey-dark mt-1">
              {str("We'll never share your email with anyone else.")}
            </small>
          </div>
          <div className="mt-3">
            <label htmlFor="sign-up-form__password">
              {str("Password")}
            </label>
            <input
              type_="password"
              className="rounded bg-grey-light focus:bg-grey-lighter p-2 mt-2 w-full"
              id="sign-up-form__password"
              required=true
            />
          </div>
          <div className="mt-3">
            <label htmlFor="sign-up-form__password-confirmation">
              {str("Confirm Password")}
            </label>
            <input
              type_="password"
              className="rounded bg-grey-light focus:bg-grey-lighter p-2 mt-2 w-full"
              id="sign-up-form__password-confirmation"
              placeholder="Again, to be sure"
              required=true
            />
            <small className="block text-grey-dark mt-1">
              {
                "Please store this password in your personal password manager. "
                |> str
              }
            </small>
          </div>
          <button
            type_="submit"
            disabled=inProgress
            className="mt-5 btn bg-green hover:bg-green-dark text-white">
            {str("Sign Up")}
          </button>
          <a
            onClick={gotoSignIn(appSend)}
            className="mt-5 ml-2 btn border hover:bg-grey-light cursor-pointer">
            {str("Sign In")}
          </a>
        </form>
      </div>
    </div>
  </div>;
};