type t = UnsignedByteArray.t;

let create = () : t => {
  let array = UnsignedByteArray.createWithLength(32);
  /* getRandomValues modifies the array, and so returns unit. */
  array |> UnsignedByteArray.getRandomValues;
  array;
};

let toString = (t: t) => t |> UnsignedByteArray.toBase64String;

/**
 * TODO: If the length of string is less than 16, create a random value instead
 * of deriving from it. This is to protect against the user inputting single
 * values, removing whole text, etc.
 */
let recreate = s : Js.Promise.t(t) =>
  s
  |> UnsignedByteArray.encode
  |> UnsignedByteArray.digest("SHA-256")
  |> Js.Promise.then_(arrayBuffer =>
       arrayBuffer |> UnsignedByteArray.fromArrayBuffer |> Js.Promise.resolve
     );