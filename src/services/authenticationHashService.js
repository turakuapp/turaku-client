import jsSHA from "jssha/src/sha256";

export default class AuthenticationHashService {
  constructor(password, authenticationSalt) {
    this.password = password;
    this.authenticationSalt = authenticationSalt;
  }

  authenticationHash() {
    console.log("Hashing ", this.password, this.authenticationSalt);

    let preHash = new jsSHA("SHA-256", "TEXT");
    preHash.update(this.password);
    preHash.update(this.authenticationSalt);
    return preHash.getHash("HEX");
  }
}
