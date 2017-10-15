import ApiService from "../apiService";

export default class AcceptService {
  constructor(token, invitationId) {
    this.token = token;
    this.invitationId = invitationId;
  }

  accept() {
    let api = new ApiService(this.token);
    return api.post("invitations/" + this.invitationId + "/accept");
  }
}
