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
  Js.Promise.t(string) =
  "decrypt";

type encryptionKey =
  | EncryptionHashAsKey(EncryptionHash.t)
  | TeamPasswordAsKey(TeamPassword.t);

let encrypt = (k, s) => {
  let iv = InitializationVector.create();
  let algorithm: encryptionAlgorithm = {"name": "AES-CBC", "iv": iv};
  (
    switch (k) {
    | EncryptionHashAsKey(unsignedByteArray)
    | TeamPasswordAsKey(unsignedByteArray) =>
      unsignedByteArray
      |> CryptographicKey.create
      |> Js.Promise.then_(cryptoKey =>
           s
           |> UnsignedByteArray.encode
           |> subtleEncrypt(algorithm, cryptoKey)
         )
    }
  )
  |> Js.Promise.then_(ciphertext =>
       {iv, ciphertext: ciphertext |> CipherText.create} |> Js.Promise.resolve
     );
};

let decrypt = (k, t) : string => "Hello";

let create = (iv, ciphertext) => {iv, ciphertext};