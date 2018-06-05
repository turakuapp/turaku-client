type t;

type id = string;

let getName: t => string;

let getId: t => string;

let create: (id, string, EncryptedData.t) => t;