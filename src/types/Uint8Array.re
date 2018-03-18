type t;

[@bs.new] external createWithLength : int => t = "Uint8Array";

[@bs.new] external createWithBuffer : ArrayBuffer.t => t = "Uint8Array";