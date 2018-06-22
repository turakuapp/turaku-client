type t;
type id = Invitation.id;

let id: t => id;
let name: t => option(string);
let email: t => Email.t;
let create: (id, Email.t, option(string)) => t;