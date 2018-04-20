type t;

/* TODO: Clean up this toHexString function to remove raw JS. */
let toHexString = (buffer: t) => {
  let rawImplementation: t => string = [%raw
    {|
    function(jsArray) {
      return Array.prototype.map
        .call(new Uint8Array(jsArray), x => ("00" + x.toString(16)).slice(-2))
        .join("")
    }
  |}
  ];
  rawImplementation(buffer);
};