/** Authentication hash is the password (Base64 string)
 * sent to the server to authenticate the user.
 *
 * TODO: AuthenticationHash.t should probably be a Hash.t
 * or an UnsignedByteArray.t (like EncryptionHash.t)
 * internally, not a string. */
type t = string;

/* TODO: Hash.create should return an opaque type t. There should be a toString method to return a string. */
let create = (password, authenticationSalt: Salt.t): Js.Promise.t(t) =>
  Hash.create(password, authenticationSalt)
  |> Js.Promise.then_(arrayBuffer =>
       arrayBuffer
       |> UnsignedByteArray.fromArrayBuffer
       |> UnsignedByteArray.toBase64String
       |> Js.Promise.resolve
     );
