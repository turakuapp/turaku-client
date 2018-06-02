type credentials = {
  accessToken: AccessToken.t,
  encryptionHash: EncryptionHash.t,
};

type t =
  | SignedOut
  | SignedIn(credentials);

let shouldRestore = () : bool => true;

let create = (accessToken, encryptionHash) =>
  SignedIn({accessToken, encryptionHash});

let signedOut = () => SignedOut;

/* Save the token in storage to allow it to be restored without signing in again on a page reload. */
let saveInLocalStorage = t =>
  switch (t) {
  | SignedOut => ()
  | SignedIn(c) =>
    Dom.Storage.setItem("token", c.accessToken, Dom.Storage.sessionStorage);
    Dom.Storage.setItem(
      "encryptionHash",
      c.encryptionHash,
      Dom.Storage.sessionStorage,
    );
  };

let attemptRestoration = () => SignedOut;