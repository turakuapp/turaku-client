[@bs.module "base64-js"]
external byteArrayToBase64String : Uint8Array.t => string = "fromByteArray";

[@bs.val]
external getRandomValues : Uint8Array.t => Uint8Array.t =
  "window.crypto.getRandomValues";

let randomUnsignedArray = () =>
  Uint8Array.createWithLength(32) |> getRandomValues;

let randomBase64String = () =>
  randomUnsignedArray() |> byteArrayToBase64String;