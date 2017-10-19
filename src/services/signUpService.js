import ApiService from "./apiService";
import HashService from "./hashService";
import _ from "lodash";

export default class SignUpService {
  saltPossibleCharacters = "1234567890abcdef";

  signUp(name, email, password) {
    let api = new ApiService();
    let salt = this.authenticationSalt();
    let hashService = new HashService(password, salt);

    return hashService.hexHash().then(hash => {
      return api
        .post("users", {
          user: {
            name: name,
            email: email,
            password: hash,
            authentication_salt: salt
          }
        })
        .then(response => {
          console.log(response, "success");
        })
        .catch(response => {
          console.log(response, "failure");
          // TODO: What should be returned if sign up fails?
          return Promise.reject(
            new Error("Response from API indicated a failure.")
          );
        });
    });
  }

  authenticationSalt() {
    if (_.isString(this.storedAuthenticationSalt)) {
      return this.storedAuthenticationSalt;
    }

    this.storedAuthenticationSalt = "";

    for (var i = 0; i < 64; i++) {
      this.storedAuthenticationSalt += this.saltPossibleCharacters.charAt(
        Math.floor(Math.random() * this.saltPossibleCharacters.length)
      );
    }

    return this.storedAuthenticationSalt;
  }
}
