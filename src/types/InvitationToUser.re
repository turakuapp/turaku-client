type t = {
  id,
  email: string,
  name: option(string),
}
and id = Invitation.id;

let id = t => t.id;

let email = t => t.email;

let name = t => t.name;

let create = (id, email, name) => {id, email, name};
