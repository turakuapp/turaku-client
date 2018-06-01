type credentials = {
  token: AccessToken.t,
  encryptionHash: EncryptionHash.t,
};

type t =
  | SignedOut
  | SignedIn(credentials);

let shouldRestore = () : bool => true;

/* Dom.Storage.sessionStorage */
let restore = (session: t) => ();

let create = (~token, ~encryptionHash) => SignedIn({token, encryptionHash});

let signedOut = () => SignedOut;