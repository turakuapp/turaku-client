type t;

type id = string;

let id: t => id;

let title: t => string;

let fields: t => list(Field.t);

module Codec: {let decode: (id, Js.Json.t) => t;};