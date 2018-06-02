type t;

let create: (AccessToken.t, EncryptionHash.t) => t;

let signedOut: unit => t;

let attemptRestoration: unit => t;

let saveInLocalStorage: (string, AccessToken.t) => unit;