type t = {
  id: int,
  name: string,
  email: string,
  encryptionSalt: string,
};

let decode = json =>
  Json.Decode.{
    id: json |> field("id", int),
    name: json |> field("name", string),
    email: json |> field("email", string),
    encryptionSalt: json |> field("encryption_salt", string),
  };

let encryptionSalt = user => user.encryptionSalt;