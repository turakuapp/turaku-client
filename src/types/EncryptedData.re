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

[@bs.val] [@bs.scope ("window", "crypto", "subtle", "encrypt")]
external subtleEncrypt :
  (encryptionAlgorithm, CryptographicKey.t, UnsignedByteArray.t) =>
  CipherText.t =
  "encrypt";

[@bs.val] [@bs.scope ("window", "crypto", "subtle", "encrypt")]
external subtleDecrypt : (
  encryptionAlgorithm,
  CryptographicKey.t,
  UnsignedByteArray.t,
) =
  "decrypt";

type encryptionKey =
  | EncryptionHashAsKey(EncryptionHash.t)
  | TeamPasswordAsKey(TeamPassword.t);

let encrypt = (k, s) : t => {
  let cryptoKey =
    switch (k) {
    | EncryptionHashAsKey(unsignedByteArray)
    | TeamPasswordAsKey(unsignedByteArray) =>
      CryptographicKey.create(unsignedByteArray)
    };
  let iv = InitializationVector.create();
  let algorithm: encryptionAlgorithm = {"name": "AES-CBC", "iv": iv};
  {
    iv,
    ciphertext:
      s |> UnsignedByteArray.encode |> subtleEncrypt(algorithm, cryptoKey),
  };
};

let decrypt = (k, t) : string => "Hello";

let create = (iv, ciphertext) => {iv, ciphertext};