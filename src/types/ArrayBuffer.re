type t;

let toHexString = (buffer: t) => {
  /* ReasonML is unaware that _jsArray is being used in raw JS. An underscored variable here avoids warning in the function declaration. */
  let _jsArray = buffer;
  let hexString: string = [%raw
    {|
       Array.prototype.map
           .call(new Uint8Array(_jsArray), x => ("00" + x.toString(16)).slice(-2))
           .join("")
       |}
  ];
  hexString;
};