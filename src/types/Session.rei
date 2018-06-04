type credentials;

type t =
  | SignedOut
  | SignedIn(credentials);

let create: (AccessToken.t, EncryptionHash.t) => t;

let signOut: unit => t;

let signedOut: t => bool;

let attemptRestoration: unit => t;

let saveInLocalStorage: t => unit;

let getCredentials: t => option(credentials);

let getAccessToken: credentials => AccessToken.t;

let getEncryptionHash: credentials => EncryptionHash.t;