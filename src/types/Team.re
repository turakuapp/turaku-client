type t = {
  id: int,
  name: string,
  encryptedPassword: string,
};

let decode = json =>
  Json.Decode.{
    id: json |> field("id", int),
    name: json |> field("name", string),
    encryptedPassword: json |> field("encrypted_password", string),
  };

let name = team => team.name;