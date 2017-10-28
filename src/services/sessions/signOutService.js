import ApiService from "../apiService";

export default class RestoreService {
  constructor(token) {
    this.token = token;
  }

  async signOut() {
    await new ApiService(this.token).delete("session");
    this.deleteStoredSessionData();
  }

  deleteStoredSessionData() {
    sessionStorage.clear();
  }
}
