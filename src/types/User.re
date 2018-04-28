type t = {
  id: int,
  name: string,
  email: string,
};

module Codec = {
  let decode = json =>
    Json.Decode.{
      id: json |> field("id", int),
      name: json |> field("name", string),
      email: json |> field("email", string),
    };
};