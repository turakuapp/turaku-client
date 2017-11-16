import ApiService from "../apiService";

export default class ListService {
  constructor(token, key, teamId, appState, setAppState) {
    this.token = token;
    this.key = key;
    this.teamId = teamId;
    this.appState = appState;
    this.setAppState = setAppState;
  }

  async list() {
    const api = new ApiService(this.token);
    const listPath = "teams/" + this.teamId + "/entries";
    const response = await api.get(listPath);

    return response.entries;
  }
}
