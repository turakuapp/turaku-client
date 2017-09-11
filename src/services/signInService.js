import ApiService from "./apiService";

export default class SignInService {
  signIn(email, password) {
    let api = new ApiService();

    return api
      .post("sessions", { email: email, password: password })
      .then(response => {
        console.log(response, "success");
        return Promise.resolve(response["token"]);
      })
      .catch(response => {
        console.log(response, "failure");
        // TODO: What should be returned if sign in fails?
        return Promise.reject(
          new Error("Response from API indicated a failure.")
        );
      });
  }
}
