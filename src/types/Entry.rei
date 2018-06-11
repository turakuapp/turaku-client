type t;

type id = string;

let getId: t => id;

let getTitle: t => string;

let getFields: t => list(Field.t);

module Codec: {let decode: (id, Js.Json.t) => t;};