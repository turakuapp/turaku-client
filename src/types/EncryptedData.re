type t = {
  iv: InitializationVector.t,
  ciphertext: CipherText.t,
};

type encryptionAlgorithm = {
  .
  "name": string,
  "iv": InitializationVector.t,
};

[@bs.val] [@bs.scope ("window", "crypto", "subtle")]
external subtleEncrypt:
  (encryptionAlgorithm, CryptographicKey.t, UnsignedByteArray.t) =>
  Js.Promise.t(ArrayBuffer.t) =
  "encrypt";

[@bs.val] [@bs.scope ("window", "crypto", "subtle")]
external subtleDecrypt:
  (encryptionAlgorithm, CryptographicKey.t, UnsignedByteArray.t) =>
  Js.Promise.t(UnsignedByteArray.t) =
  "decrypt";

let algorithm = iv: encryptionAlgorithm => {"name": "AES-CBC", "iv": iv};

let encrypt = (key, plaintext) => {
  let iv = InitializationVector.create();
  key
  |> CryptographicKey.create
  |> Js.Promise.then_(cryptoKey =>
       plaintext
       |> TextEncoder.encode
       |> subtleEncrypt(iv |> algorithm, cryptoKey)
     )
  |> Js.Promise.then_(ciphertext =>
       {iv, ciphertext: ciphertext |> CipherText.create} |> Js.Promise.resolve
     );
};

let decrypt = (key, t) =>
  key
  |> CryptographicKey.create
  |> Js.Promise.then_(cryptoKey =>
       t.ciphertext
       |> CipherText.toArrayBuffer
       |> subtleDecrypt(t.iv |> algorithm, cryptoKey)
     )
  |> Js.Promise.then_(byteArray =>
       byteArray |> TextDecoder.decode |> Js.Promise.resolve
     );

let create = (iv, ciphertext) => {iv, ciphertext};
let iv = t => t.iv;
let ciphertext = t => t.ciphertext;
