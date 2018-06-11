type t = {. [@bs.meth] "encode": string => UnsignedByteArray.t};

[@bs.new] external create : string => t = "TextEncoder";

let encode = s => {
  let encoder = create("utf-8");
  encoder##encode(s);
};