import ApiService from "./apiService";
import AuthenticationHashService from "./authenticationHashService";
import _ from "lodash";

export default class SignUpService {
  saltPossibleCharacters = "1234567890abcdef";

  signUp(name, email, password) {
    let api = new ApiService();
    let hashService = new AuthenticationHashService(
      password,
      this.authenticationSalt()
    );

    return api
      .post("users", {
        user: {
          name: name,
          email: email,
          password: hashService.authenticationHash(),
          authentication_salt: this.authenticationSalt()
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
