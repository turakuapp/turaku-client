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

let id = t => t.id;

let title = t => t.title;

let fields = t => t.fields;