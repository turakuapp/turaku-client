type t;

let create: (~token: AccessToken.t, ~encryptionHash: EncryptionHash.t) => t;

let signedOut: unit => t;