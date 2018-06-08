type t;

[@bs.val] [@bs.scope ("window", "crypto", "subtle")]
external subtleImportKey :
  (string, UnsignedByteArray.t, string, bool, array(string)) =>
  Js.Promise.t(t) =
  "importKey";

/** A CryptoKey can be generated from one of these sources. */
type source =
  | EncryptionHashAsKey(EncryptionHash.t)
  | TeamPasswordAsKey(TeamPassword.t);

let keyFromEncryptionHash = k => EncryptionHashAsKey(k);

let keyFromTeamPassword = k => TeamPasswordAsKey(k);

let create = key => {
  let keyData =
    switch (key) {
    | EncryptionHashAsKey(k)
    | TeamPasswordAsKey(k) => k
    };
  subtleImportKey(
    "raw",
    keyData,
    "AES-CBC",
    false,
    [|"encrypt", "decrypt"|],
  );
};