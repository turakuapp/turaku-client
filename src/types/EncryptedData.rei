type t;

let create: (InitializationVector.t, CipherText.t) => t;

let iv: t => InitializationVector.t;
let ciphertext: t => CipherText.t;

let decrypt: (CryptographicKey.source, t) => Js.Promise.t(string);
let encrypt: (CryptographicKey.source, string) => Js.Promise.t(t);