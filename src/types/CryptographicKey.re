type t;

[@bs.val] [@bs.scope ("window", "crypto", "subtle")]
external subtleImportKey :
  (string, UnsignedByteArray.t, string, bool, array(string)) => t =
  "";

let create = (keyData: UnsignedByteArray.t) : t =>
  subtleImportKey(
    "raw",
    keyData,
    "AES-CBC",
    false,
    [|"encrypt", "decrypt"|],
  );