type t;

let signIn: (~token: string, ~encryptionHash: string) => t;

let signOut: unit => t;