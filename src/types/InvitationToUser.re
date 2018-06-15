type t = {
  id,
  email: Email.t,
  name: option(string),
}
and id = string;

let id = t => t.id;

let email = t => t.email;

let name = t => t.name;

let create = (id, email, name) => {id, email, name};