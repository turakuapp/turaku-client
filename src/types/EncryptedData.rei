type t;

type iv = string;

type ciphertext = string;

let create: (iv, ciphertext) => t;