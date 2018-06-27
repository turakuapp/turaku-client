type t;

module Codec: {let decode: Js.Json.t => t;};

let key: t => string;
let value: t => string;

let shouldBeHidden: t => bool;

let editValue: (string, t) => t;

let forNewEntry: unit => list(t);