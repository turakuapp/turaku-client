type t = string;

let create = (incomingString, salt: Salt.t) : Js.Promise.t(t) =>
  incomingString
  ++ salt
  |> Uint8Array.encode
  |> Uint8Array.digest("SHA-256")
  |> Js.Promise.then_(arrayBuffer =>
       arrayBuffer |> ArrayBuffer.toHexString |> Js.Promise.resolve
     );