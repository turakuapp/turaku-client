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

let signIn = (~token, ~encryptionHash) => SignedIn({token, encryptionHash});

let signOut = () => SignedOut;