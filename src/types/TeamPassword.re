type t = UnsignedByteArray.t;

let create = () : t => {
  let array = UnsignedByteArray.createWithLength(32);
  /* getRandomValues modifies the array, and so returns unit. */
  array |> UnsignedByteArray.getRandomValues;
  array;
};

let toString = (t: t) => t |> UnsignedByteArray.toBase64String;

let recreate = s : Js.Promise.t(t) =>
  s
  |> UnsignedByteArray.encode
  |> UnsignedByteArray.digest("SHA-256")
  |> Js.Promise.then_(arrayBuffer =>
       arrayBuffer |> UnsignedByteArray.fromArrayBuffer |> Js.Promise.resolve
     );