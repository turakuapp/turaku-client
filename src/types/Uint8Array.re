type t;

[@bs.new] external createWithLength : int => t = "Uint8Array";

[@bs.new] external createWithBuffer : ArrayBuffer.t => t = "Uint8Array";

/* let toHexString = [%raw
  {|
    function(buffer) {
      return Array.prototype.map
        .call(new Uint8Array(buffer), x => ("00" + x.toString(16)).slice(-2))
        .join("");
    }
  |}
]; */

let toHexString = t => string {
  [%raw {|
    Array.prototype.map
        .call(new Uint8Array(buffer), x => ("00" + x.toString(16)).slice(-2))
        .join("")
    |}
};