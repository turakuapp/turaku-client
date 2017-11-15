import ApiService from "../apiService";
import CryptoService from "../cryptoService";

export default class CreateService {
  constructor(token, key, entry, teamId) {
    this.token = token;
    this.key = key;
    this.entry = entry;
    this.teamId = teamId;
  }

  async execute() {
    console.log("entries/CreateService#execute");

    // Create the encrypted entry.
    const crypto = new CryptoService(this.key);
    const encryptedEntry = await crypto.encrypt(this.entry);

    // Send the encrypted entry to the server.
    const api = new ApiService(this.token);
    const response = await api.post("entries", {
      entry: { team_id: this.teamId, encrypted_data: encryptedEntry }
    });

    return response.id;
  }
}
