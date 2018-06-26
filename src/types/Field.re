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

let key = t => t.key;
let value = t => t.value;

let editValue = (v, t) => {...t, value: v};

let forNewEntry = () => [
  {kind: TextField, key: "URL", value: ""},
  {kind: PasswordField, key: "Password", value: ""},
  {kind: TextField, key: "Notes", value: ""},
];