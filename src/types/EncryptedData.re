module InitializationVector = {
  type t = UnsignedByteArray.t;
  let create = () : t => {
    let iv = UnsignedByteArray.createWithLength(16);
    iv |> UnsignedByteArray.getRandomValues;
    iv;
  };
  let toString = (t: t) => t |> UnsignedByteArray.toBase64String;
  let fromString = s : t => s |> UnsignedByteArray.fromBase64String;
};

module CipherText = {
  type t = ArrayBuffer.t;
  let create = (t: ArrayBuffer.t) : t => t;
  let toString = (t: t) =>
    t |> UnsignedByteArray.fromArrayBuffer |> UnsignedByteArray.toBase64String;
  let fromString = s : t => s |> UnsignedByteArray.fromBase64String;
};

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
external subtleEncrypt :
  (encryptionAlgorithm, CryptographicKey.t, UnsignedByteArray.t) =>
  Js.Promise.t(CipherText.t) =
  "encrypt";

[@bs.val] [@bs.scope ("window", "crypto", "subtle")]
external subtleDecrypt :
  (encryptionAlgorithm, CryptographicKey.t, UnsignedByteArray.t) =>
  Js.Promise.t(UnsignedByteArray.t) =
  "decrypt";

let algorithm = iv : encryptionAlgorithm => {"name": "AES-CBC", "iv": iv};

let encrypt = (key, plaintext) => {
  let iv = InitializationVector.create();
  key
  |> CryptographicKey.create
  |> Js.Promise.then_(cryptoKey =>
       plaintext
       |> UnsignedByteArray.encode
       |> subtleEncrypt(iv |> algorithm, cryptoKey)
     )
  |> Js.Promise.then_(ciphertext =>
       {iv, ciphertext: ciphertext |> CipherText.create} |> Js.Promise.resolve
     );
};

let decrypt = (key, t) => {
  Js.log2(key, t);
  key
  |> CryptographicKey.create
  |> Js.Promise.then_(cryptoKey => {
       Js.log(cryptoKey);
       t.ciphertext |> subtleDecrypt(t.iv |> algorithm, cryptoKey);
     })
  |> Js.Promise.then_(byteArray =>
       byteArray |> UnsignedByteArray.decode |> Js.Promise.resolve
     );
};

let create = (iv, ciphertext) => {iv, ciphertext};