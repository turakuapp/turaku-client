type t = {. [@bs.meth] "encode": string => ArrayBuffer.t};

[@bs.new] external create : string => t = "TextEncoder";