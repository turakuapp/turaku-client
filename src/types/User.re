type t = {
  id,
  name: string,
  email: Email.t,
  encryptionSalt: string,
}
and id = string;

let decode = json =>
  Json.Decode.{
    id: json |> field("id", string),
    name: json |> field("name", string),
    email: json |> field("email", string) |> Email.create,
    encryptionSalt: json |> field("encryption_salt", string),
  };

let encryptionSalt = t => t.encryptionSalt;

let id = t => t.id;

let email = t => t.email;

let name = t => t.name;
