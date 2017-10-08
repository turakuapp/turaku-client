import ApiService from "../apiService";

export default class RestoreService {
  constructor(token) {
    this.token = token;
  }

  signOut() {
    let api = new ApiService(this.token);
    let that = this;

    return api
      .delete("session")
      .then(() => {
        that.deleteStoredSessionData();
        return Promise.resolve(true);
      })
      .catch(response => {
        console.log(response, "DELETE session -> failure");

        if (response.exception === "TokenAuthenticationFailedException") {
          that.deleteStoredSessionData();
        } else {
          return Promise.reject(
            new Error("Response from API indicated an unknown failure.")
          );
        }
      });
  }

  deleteStoredSessionData() {
    sessionStorage.clear();
  }
}
