type t;

module Codec: {let decode: Js.Json.t => t;};

let getKey: t => string;