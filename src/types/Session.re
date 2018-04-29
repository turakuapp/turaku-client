type credentials = {
  token: string,
  encryptionHash: string,
};

type t =
  | SignedOut
  | SignedIn(credentials);

let shouldRestore = () : bool => true;

/* Dom.Storage.sessionStorage */
let restore = (session: t) => ();