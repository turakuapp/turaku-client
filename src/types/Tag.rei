type t;

type id = string;

let create: (id, ~name: string, ~nameHash: string) => t;

let id: t => id;
let name: t => string;