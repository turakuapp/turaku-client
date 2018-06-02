type credentials = {
  token: AccessToken.t,
  encryptionHash: EncryptionHash.t,
};

type t =
  | SignedOut
  | SignedIn(credentials);

let shouldRestore = () : bool => true;

let create = (token, encryptionHash) => SignedIn({token, encryptionHash});

let signedOut = () => SignedOut;

/* Save the token in storage to allow it to be restored without signing in again on a page reload. */
let saveInLocalStorage = (email, token) => {
  Dom.Storage.setItem("sessionEmail", email, Dom.Storage.sessionStorage);
  Dom.Storage.setItem("sessionToken", token, Dom.Storage.sessionStorage);
};

let attemptRestoration = () => {};