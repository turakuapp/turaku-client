type t = Uint8Array.t;

let create = () : t => {
  let array = Uint8Array.createWithLength(32);
  /* getRandomValues modifies the array, and so returns unit. */
  array |> Uint8Array.getRandomValues;
  array;
};

let toString = (t: t) => t |> Uint8Array.toBase64String;

let fromString = s : Js.Promise.t(t) =>
  s
  |> Uint8Array.encode
  |> Uint8Array.digest("SHA-256")
  |> Js.Promise.then_(arrayBuffer =>
       arrayBuffer |> Uint8Array.createFromArrayBuffer |> Js.Promise.resolve
     );