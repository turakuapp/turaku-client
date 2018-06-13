type t;

type id = string;

let email: t => Email.t;

let name: t => string;

let create: (id, ~teamName: string, ~userEmail: Email.t) => t;