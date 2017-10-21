import ApiService from "./apiService";
import HashService from "./hashService";

export default class SignInService {
  constructor(email, password) {
    this.email = email;
    this.password = password;
  }

  signIn() {
    return this.loadAuthenticationSalt()
      .then(authenticationSalt => {
        return this.signInWithHashedPassword(authenticationSalt);
      })
      .then(authenticationResponse => {
        // Store the authentication response to avoid losing it during the next step.
        this.resolvedObject = authenticationResponse;

        return this.createEncryptionHash(authenticationResponse.encryptionSalt);
      })
      .then(hash => {
        this.resolvedObject.encryptionHash = hash;

        // Save both the token and the encryption hash in session to allow restoration.
        this.saveSession(this.resolvedObject.token, hash);

        return Promise.resolve(this.resolvedObject);
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

    let hashService = new HashService(this.password, authenticationSalt);

    return hashService.hexHash().then(hash => {
      return api
        .post("sessions", {
          session: {
            email: this.email,
            password: hash
          }
        })
        .then(response => {
          console.log(response, "POST sessions -> success");
          return Promise.resolve({
            token: response.token,
            encryptionSalt: response.user.encryption_salt,
            teams: response.teams,
            incomingInvitations: response.incoming_invitations
          });
        })
        .catch(response => {
          console.log(response, "POST sessions -> failure");
          // TODO: What should be returned if sign in fails?
          return Promise.reject(
            new Error("Response from API indicated a failure.")
          );
        });
    });
  }

  // Save the session in storage to allow it to be restored without signing in again on a page reload.
  saveSession(token, encryptionHash) {
    sessionStorage.setItem("token", token);
    sessionStorage.setItem("encryptionHash", encryptionHash);
  }

  createEncryptionHash(encryptionSalt) {
    // Create and store encryption hash.
    let hashService = new HashService(this.password, encryptionSalt);

    return hashService.hexHash();
  }
}
