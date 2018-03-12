type unsignedIntArray;

[@bs.new]
external createUnsignedIntArray : int => unsignedIntArray = "Uint8Array";

[@bs.module "base64-js"]
external byteArrayToBase64String : unsignedIntArray => string =
  "fromByteArray";

[@bs.val]
external getRandomValues : unsignedIntArray => unsignedIntArray =
  "window.crypto.getRandomValues";

let randomUnsignedArray = () => createUnsignedIntArray(32) |> getRandomValues;

let randomBase64String = () =>
  randomUnsignedArray() |> byteArrayToBase64String;