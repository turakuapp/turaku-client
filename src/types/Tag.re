type t = {
  id,
  name: string,
  nameHash: string,
}
and id = string;

let create = (id, ~name, ~nameHash) => {id, name, nameHash};

let id = t => t.id;
let name = t => t.name;