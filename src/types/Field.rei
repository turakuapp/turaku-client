type t;

module Codec: {let decode: Js.Json.t => t;};

let key: t => string;
let value: t => string;