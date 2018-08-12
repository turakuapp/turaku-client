type t = {
  id,
  name: string,
  email: Email.t,
}
and id = User.id;

let create = (id, name, email) => {id, name, email};

let id = t => t.id;

let email = t => t.email;

let name = t => t.name;
