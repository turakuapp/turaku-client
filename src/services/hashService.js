import _ from "lodash";

export default class HashService {
  constructor(incomingString, salt) {
    this.incomingString = incomingString;
    this.salt = salt;
  }

  hexHash() {
    console.log("Hashing ", this.password, this.authenticationSalt);

    return window.crypto.subtle
      .digest("SHA-256", this.saltedArray())
      .then(hash => {
        return Promise.resolve(this.arrayBufferToHexString(hash));
      });
  }

  arrayBufferToHexString(buffer) {
    return Array.prototype.map
      .call(new Uint8Array(buffer), x => ("00" + x.toString(16)).slice(-2))
      .join("");
  }

  saltedArray() {
    let encoder = new TextEncoder("utf-8");
    return encoder.encode(this.saltedString());
  }

  saltedString() {
    if (_.isString(this.salt)) {
      return this.incomingString + this.salt;
    }

    return this.incomingString;
  }
}
