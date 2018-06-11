type t = {. [@bs.meth] "decode": UnsignedByteArray.t => string};

[@bs.new] external create : string => t = "TextDecoder";

let decode = unsignedByteArray => {
  let decoder = create("utf-8");
  decoder##decode(unsignedByteArray);
};