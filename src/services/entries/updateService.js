import ApiService from "../apiService";
import CryptoService from "../cryptoService";

export default class UpdateService {
  constructor(token, key, entryId, entry, tagIds) {
    this.token = token;
    this.key = key;
    this.entryId = entryId;
    this.entry = entry;
    this.tagIds = tagIds;
  }

  async execute() {
    console.log("entries/UpdateService#execute");

    // Create the encrypted entry.
    const crypto = new CryptoService(this.key, true);
    const encryptedEntry = await crypto.encrypt(this.entry);

    // Send the encrypted entry to the server.
    const api = new ApiService(this.token);
    const response = await api.patch(`entries/${this.entryId}`, {
      entry: { encrypted_data: encryptedEntry, tag_ids: this.tagIds }
    });

    return response.id;
  }
}
