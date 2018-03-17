let saltedString = (incomingString: string, salt: option(string)) =>
  switch salt {
  | Some(salt) => incomingString ++ salt
  | None => incomingString
  };

let saltedArray = (incomingString, salt) => {
  let encoder = TextEncoder.create("utf-8");
  encoder##encode(saltedString(incomingString, salt));
};

let hash = (incomingString, ~salt=?, ()) =>
  CryptoUtils.digest("SHA-256", saltedArray(incomingString, salt));

let hexHash = (incomingString, ~salt=?, ()) =>
  (
    switch salt {
    | Some(salt) => hash(incomingString, ~salt, ())
    | None => hash(incomingString, ())
    }
  )
  |> Js.Promise.then_(digestArray =>
       Js.Promise.resolve(arrayBufferToHexString(digestArray))
     );
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