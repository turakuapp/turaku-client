type t;

type id = string;

let name: t => string;

let create: (id, string, EncryptedData.t) => t;