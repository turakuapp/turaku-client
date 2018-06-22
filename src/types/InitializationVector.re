type t = UnsignedByteArray.t;
let create = () : t => {
  let iv = UnsignedByteArray.createWithLength(16);
  iv |> UnsignedByteArray.getRandomValues;
  iv;
};
let toString = (t: t) => t |> UnsignedByteArray.toBase64String;
let fromString = s : t => s |> UnsignedByteArray.fromBase64String;