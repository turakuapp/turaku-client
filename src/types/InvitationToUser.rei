type t;
type id = Invitation.id;

let id: t => id;
let name: t => option(string);
let email: t => string;
let create: (id, string, option(string)) => t;
