/** Encryption hash is the password (256 bits) used the encrypt the team passwords that a user has access to. */
type t = UnsignedByteArray.t;

let fromString = base64String : t =>
  base64String |> UnsignedByteArray.fromBase64String;

let toString = (t: t) => t |> UnsignedByteArray.toBase64String;

let create = (password, encryptionSalt: Salt.t) : Js.Promise.t(t) =>
  Hash.create(password, encryptionSalt)
  |> Js.Promise.then_(arrayBuffer =>
       arrayBuffer |> UnsignedByteArray.fromArrayBuffer |> Js.Promise.resolve
     );