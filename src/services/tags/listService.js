import ApiService from "../apiService";

export default class ListService {
  constructor(token, teamId) {
    this.token = token;
    this.teamId = teamId;
  }

  async list() {
    const api = new ApiService(this.token);
    const response = await api.get("tags", { team_id: this.teamId });

    return response.tags.reduce((modifiedTags, tag) => {
      modifiedTags.push({
        id: tag.id,
        nameHash: tag.name_hash,
        encryptedName: tag.encrypted_name
      });

      return modifiedTags;
    }, []);
  }
}
