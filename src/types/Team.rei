type t;

type id = string;

let getName: t => string;

let getId: t => string;

let getEncryptedPassword: t => EncryptedData.t;

let create: (id, string, EncryptedData.t) => t;