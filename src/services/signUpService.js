import ApiService from "./apiService";

export default class SignUpService {
  saltPossibleCharacters = "1234567890abcdef";

  signUp(name, email, password) {
    let api = new ApiService();

    return api
      .post("users", {
        user: {
          name: name,
          email: email,
          password: password,
          authentication_salt: this.newAuthenticationSalt()
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

  newAuthenticationSalt() {
    let salt = "";

    for (var i = 0; i < 64; i++) {
      salt += this.saltPossibleCharacters.charAt(
        Math.floor(Math.random() * this.saltPossibleCharacters.length)
      );
    }

    return salt;
  }
}
