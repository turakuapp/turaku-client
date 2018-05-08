type t;

type purpose =
  | SignUp
  | SignIn
  | GetAuthenticationSalt;

let create: (~purpose: purpose) => t;

let createWithToken: (~purpose: purpose, ~token: string) => t;

let fetch: (~apiRequest: t, ~params: Js.Json.t) => Js.Promise.t(Js.Json.t);