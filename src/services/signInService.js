import ApiService from "./apiService";
import AuthenticationHashService from "./authenticationHashService";

export default class SignInService {
  constructor(email, password) {
    this.email = email;
    this.password = password;
  }

  signIn() {
    return this.loadAuthenticationSalt().then(authenticationSalt => {
      return this.signInWithHashedPassword(authenticationSalt);
    });
  }

  loadAuthenticationSalt() {
    let api = new ApiService();

    return api
      .get("users/authentication_salt", { email: this.email })
      .then(response => {
        console.log(response, "GET users/authentication_salt -> success");

        return Promise.resolve(response.salt);
      })
      .catch(response => {
        console.log(response, "GET users/authentication_salt -> failure");
        // TODO: What should be returned if load authentication fails?
        return Promise.reject(
          new Error("Response from API indicated a failure.")
        );
      });
  }

  signInWithHashedPassword(authenticationSalt) {
    let api = new ApiService();

    let hashService = new AuthenticationHashService(
      this.password,
      authenticationSalt
    );

    return api
      .post("sessions", {
        session: {
          email: this.email,
          password: hashService.authenticationHash()
        }
      })
      .then(response => {
        console.log(response, "POST sessions -> success");
        return Promise.resolve({
          token: response.token,
          encryptionSalt: response.user.encryption_salt
        });
      })
      .catch(response => {
        console.log(response, "POST sessions -> failure");
        // TODO: What should be returned if sign in fails?
        return Promise.reject(
          new Error("Response from API indicated a failure.")
        );
      });
  }
}