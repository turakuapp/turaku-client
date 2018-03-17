[@bs.val] [@bs.scope ("window", "crypto", "subtle")]
external digest : (string, ArrayBuffer.t) => Js.Promise.t(ArrayBuffer.t) = "";