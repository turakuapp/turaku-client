type t = string;

let create = (incomingString, salt: Salt.t) : Js.Promise.t(t) =>
  incomingString
  ++ salt
  |> UnsignedByteArray.encode
  |> UnsignedByteArray.digest("SHA-256")
  |> Js.Promise.then_(arrayBuffer =>
       arrayBuffer |> ArrayBuffer.toHexString |> Js.Promise.resolve
     );