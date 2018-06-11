type t = {
  id,
  title: string,
  fields: list(Field.t),
}
and id = string;

module Codec = {
  let decode = (id, json) =>
    Json.Decode.{
      id,
      title: json |> field("title", string),
      fields: json |> field("fields", list(Field.Codec.decode)),
    };
};

let getId = t => t.id;

let getTitle = t => t.title;

let getFields = t => t.fields;