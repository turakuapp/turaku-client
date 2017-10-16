import ApiService from "../apiService";

export default class CreateService {
  constructor(token) {
    this.token = token;
  }

  invite(email, teamId) {
    console.log("invitations/CreateService#invite");

    let api = new ApiService(this.token);

    return api
      .post("invitations", {
        invitation: {
          team_id: teamId,
          email: email
        }
      })
      .then(response => {
        console.log(response, "success");
        return Promise.resolve(response.invited_user);
      })
      .catch(response => {
        console.log(response, "failure");
        // TODO: What should be returned if invite user fails?
        return Promise.reject(
          new Error("Response from API indicated a failure.")
        );
      });
  }
}
