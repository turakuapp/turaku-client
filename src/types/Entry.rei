type t;

type id = string;

let newUnsaved: int => t;

let id: t => id;
let title: t => string;
let fields: t => list(Field.t);

let unsaved: t => bool;
let unpersisted: t => bool;

let editTitle: (string, t) => t;

/** editField takes a new field, the index at which it should be placed / replaced, and the entry. */
let editField: (Field.t, int, t) => t;

module Codec: {let decode: (id, Js.Json.t) => t;};