/** Authentication hash is the password (Base64 string) sent to the server to authenticate the user. */
type t = string;

let create = (password, authenticationSalt: Salt.t) : Js.Promise.t(t) =>
  Hash.create(password, authenticationSalt)
  |> Js.Promise.then_(arrayBuffer =>
       arrayBuffer
       |> UnsignedByteArray.fromArrayBuffer
       |> UnsignedByteArray.toBase64String
       |> Js.Promise.resolve
     );