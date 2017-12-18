import ApiService from "../apiService";

export default class CreateService {
  constructor(token, teamId, tag) {
    this.token = token;
    this.teamId = teamId;
    this.tag = tag;
  }

  async create() {
    console.log("tags/CreateService#create");

    // Send the tag data to the server for creation.
    const api = new ApiService(this.token);

    const response = await api.post("tags", {
      tag: {
        team_id: this.teamId,
        encrypted_name: this.tag.encryptedName,
        name_hash: this.tag.nameHash
      }
    });

    return {
      id: "" + response.id,
      name: this.tag.name,
      encryptedName: response.encrypted_name,
      nameHash: response.name_hash
    };
  }
}
