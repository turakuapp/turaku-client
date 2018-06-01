type iv = string;

type ciphertext = string;

type t = {
  iv,
  ciphertext,
};

let create = (iv, ciphertext) => {iv, ciphertext};