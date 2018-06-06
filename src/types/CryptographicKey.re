type t;

[@bs.val] [@bs.scope ("window", "crypto", "subtle")]
external subtleImportKey :
  (string, UnsignedByteArray.t, string, bool, array(string)) =>
  Js.Promise.t(t) =
  "importKey";

let create = keyData =>
  subtleImportKey(
    "raw",
    keyData,
    "AES-CBC",
    false,
    [|"encrypt", "decrypt"|],
  );