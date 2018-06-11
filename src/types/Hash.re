type t = ArrayBuffer.t;

let create = (incomingString, salt: Salt.t) : Js.Promise.t(t) =>
  incomingString
  ++ salt
  |> TextEncoder.encode
  |> UnsignedByteArray.digest("SHA-256");