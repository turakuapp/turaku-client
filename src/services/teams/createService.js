import ApiService from "../apiService";

export default class CreateService {
  constructor(token) {
    this.token = token;
  }

  create(name) {
    console.log("teams/CreateService#create");

    let api = new ApiService(this.token);

    return api
      .post("teams", {
        team: {
          name: name
        }
      })
      .then(response => {
        console.log(response, "success");
        return Promise.resolve(response);
      })
      .catch(response => {
        console.log(response, "failure");
        // TODO: What should be returned if create team fails?
        return Promise.reject(
          new Error("Response from API indicated a failure.")
        );
      });
  }
}
