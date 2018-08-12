type t = {
  accessToken: AccessToken.t,
  encryptionHash: EncryptionHash.t,
};

let shouldRestore = (): bool => true;

let create = (accessToken, encryptionHash) => {accessToken, encryptionHash};

let signOut = (_t: t) =>
  Dom.Storage.removeItem("credentials", Dom.Storage.sessionStorage);

/** Handles conversion a set of credentials to and from JSON, for compatibility with local storage. */
module Codec = {
  let encode = c =>
    Json.Encode.(
      object_([
        ("accessToken", c.accessToken |> string),
        (
          "encryptionHash",
          c.encryptionHash |> EncryptionHash.toString |> string,
        ),
      ])
    );
  let decode = json =>
    Json.Decode.{
      accessToken: json |> field("accessToken", string) |> AccessToken.create,
      encryptionHash:
        json |> field("encryptionHash", string) |> EncryptionHash.fromString,
    };
};

/* Save the token in storage to allow it to be restored without signing in again on a page reload. */
let saveInLocalStorage = t =>
  Dom.Storage.setItem(
    "credentials",
    t |> Codec.encode |> Js.Json.stringify,
    Dom.Storage.sessionStorage,
  );

let attemptRestoration = () =>
  switch (Dom.Storage.getItem("credentials", Dom.Storage.sessionStorage)) {
  | None => None
  | Some(c) => Some(c |> Json.parseOrRaise |> Codec.decode)
  };

let getAccessToken = c => c.accessToken;

let getCryptographicKey = session =>
  session.encryptionHash |> CryptographicKey.keyFromEncryptionHash;
