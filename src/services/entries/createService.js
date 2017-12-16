import ApiService from "../apiService";
import CryptoService from "../cryptoService";

export default class CreateService {
  constructor(token, key, teamId, entry, tagIds) {
    this.token = token;
    this.key = key;
    this.teamId = teamId;
    this.entry = entry;
    this.tagIds = tagIds;
  }

  async execute() {
    console.log("entries/CreateService#execute");

    // Create the encrypted entry.
    const crypto = new CryptoService(this.key, true);
    const encryptedEntry = await crypto.encrypt(this.entry);

    // Send the encrypted entry to the server.
    const api = new ApiService(this.token);
    const response = await api.post("entries", {
      entry: {
        team_id: this.teamId,
        encrypted_data: encryptedEntry,
        tag_ids: this.tagIds
      }
    });

    return response.id;
  }
}
