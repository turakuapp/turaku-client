type kind =
  | PasswordField
  | TextField;

type key = string;

type value = string;

type t = {
  kind,
  key,
  value,
};

module Codec = {
  let kindFromString = kindString =>
    switch (kindString) {
    | "password" => PasswordField
    | "text" => TextField
    | _ => failwith("Unexpected Field kindString encountered: " ++ kindString)
    };
  let decode = json =>
    Json.Decode.{
      kind: json |> field("kind", string) |> kindFromString,
      key: json |> field("key", string),
      value: json |> field("value", string),
    };
};

let getKey = t => t.key;