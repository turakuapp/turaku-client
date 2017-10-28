import ApiService from "../apiService";
import CryptoService from "../cryptoService";

export default class AcceptService {
  constructor(token, encryptionHash, invitationId, teamPassword) {
    this.token = token;
    this.encryptionHash = encryptionHash;
    this.invitationId = invitationId;
    this.teamPassword = teamPassword;
  }

  async accept() {
    const encryptedPassword = await new CryptoService(
      this.encryptionHash
    ).encrypt(this.teamPassword);

    return new ApiService(this.token).post(
      "invitations/" + this.invitationId + "/accept",
      { team: { encrypted_password: encryptedPassword } }
    );
  }
}
