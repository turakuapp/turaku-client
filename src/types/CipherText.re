type t = ArrayBuffer.t;
let create = (t: ArrayBuffer.t) : t => t;
let toString = (t: t) =>
  t |> UnsignedByteArray.fromArrayBuffer |> UnsignedByteArray.toBase64String;
let fromString = s : t => s |> UnsignedByteArray.fromBase64String;
let toArrayBuffer = t => t;