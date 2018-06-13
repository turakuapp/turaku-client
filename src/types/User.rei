type t;

type id = string;

let decode: Js.Json.t => t;

let id: t => id;

let name: t => string;

let email: t => Email.t;

let encryptionSalt: t => string;