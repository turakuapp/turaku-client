type t;
type id = string;
let id: t => id;
let name: t => option(string);
let email: t => Email.t;
let create: (id, Email.t, option(string)) => t;