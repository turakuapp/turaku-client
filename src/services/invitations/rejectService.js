import ApiService from "../apiService";

export default class RejectService {
  constructor(token, invitationId) {
    this.token = token;
    this.invitationId = invitationId;
  }

  reject() {
    let api = new ApiService(this.token);
    return api.delete("invitations/" + this.invitationId);
  }
}
