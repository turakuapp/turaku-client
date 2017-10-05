import ApiService from "../apiService";

export default class ListUsersService {
  constructor(team, token) {
    this.team = team;
    this.token = token;
  }

  list() {
    console.log("teams/ListUsersService#users");

    let api = new ApiService(this.token);

    return api.get("teams/" + this.team.id + "/users").then(response => {
      console.log(response, "success");
      return Promise.resolve(response.users);
    });
  }
}
