type t = ArrayBuffer.t;

type textEncoder = {. [@bs.meth] "encode": string => t};

[@bs.new] external createTextEncoder : string => textEncoder = "TextEncoder";

[@bs.new] external createWithLength : int => t = "Uint8Array";

[@bs.new] external fromArrayBuffer : ArrayBuffer.t => t = "Uint8Array";

[@bs.module "base64-js"]
external toBase64String : t => string = "fromByteArray";

[@bs.module "base64-js"]
external fromBase64String : string => t = "toByteArray";

[@bs.val] [@bs.scope ("window", "crypto")]
external getRandomValues : t => unit = "getRandomValues";

[@bs.val] [@bs.scope ("window", "crypto", "subtle")]
external digest : (string, t) => Js.Promise.t(ArrayBuffer.t) = "";

let encode = s => {
  let encoder = createTextEncoder("utf-8");
  encoder##encode(s);
};