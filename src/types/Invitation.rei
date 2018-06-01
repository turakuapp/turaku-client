type t;

type id = string;

let email: t => string;

let name: t => string;

let create: (id, ~teamName: string, ~userEmail: string) => t;