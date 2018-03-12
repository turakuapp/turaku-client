type arrayBuffer;

[@bs.val] [@bs.scope ("window", "crypto", "subtle")]
external digest : (string, arrayBuffer) => string = "";

type textEncoder = {. [@bs.meth] "encode": string => arrayBuffer};

[@bs.new] external createTextEncoder : string => textEncoder = "TextEncoder";

let saltedString = (incomingString, salt) =>
  switch salt {
  | Some(salt) => incomingString ++ salt
  | None => incomingString
  };

let saltedArray = (incomingString, salt) => {
  let encoder = createTextEncoder("utf-8");
  encoder##encode(saltedString(incomingString, salt));
};

let hash = (incomingString, ~salt=?, ()) =>
  digest("SHA-256", saltedArray(incomingString, salt));
/* hexHash(incomingString, salt) {
     console.log("Hashing ", this.password, this.authenticationSalt);

     return this.hash().then(hash => {
       return Promise.resolve(this.arrayBufferToHexString(hash));
     });
   }

   arrayBufferToHexString(buffer) {
     return Array.prototype.map
       .call(new Uint8Array(buffer), x => ("00" + x.toString(16)).slice(-2))
       .join("");
   } */