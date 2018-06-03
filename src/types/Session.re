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

/** Handles conversion a set of credentials to and from JSON, for compatibility with local storage. */
module Codec = {
  let encode = c =>
    Json.Encode.(
      object_([
        ("accessToken", c.accessToken |> string),
        ("encryptionHash", c.encryptionHash |> string),
      ])
    );
  let decode = json =>
    Json.Decode.{
      accessToken: json |> field("accessToken", string) |> AccessToken.create,
      encryptionHash:
        json |> field("encryptionHash", string) |> EncryptionHash.create,
    };
};

/* Save the token in storage to allow it to be restored without signing in again on a page reload. */
let saveInLocalStorage = t =>
  switch (t) {
  | SignedOut => ()
  | SignedIn(c) =>
    Dom.Storage.setItem(
      "credentials",
      c |> Codec.encode |> Js.Json.stringify,
      Dom.Storage.sessionStorage,
    )
  };

let attemptRestoration = () =>
  switch (Dom.Storage.getItem("credentials", Dom.Storage.sessionStorage)) {
  | None => SignedOut
  | Some(c) =>
    let credentials = c |> Json.parseOrRaise |> Codec.decode;
    SignedIn(credentials);
  };