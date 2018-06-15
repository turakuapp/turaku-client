type t = {
  id,
  email: Email.t,
  name: option(string),
}
and id = string;