import ApiService from "../apiService";
import CryptoService from "../cryptoService";

export default class CreateService {
  constructor(token, encryptionHash) {
    this.token = token;
    this.encryptionHash = encryptionHash;
  }

  async create(name, password) {
    console.log("teams/CreateService#create");

    let encryptedPassword = await new CryptoService(
      this.encryptionHash
    ).encrypt(password);

    return await new ApiService(this.token).post("teams", {
      team: {
        name: name,
        encrypted_password: encryptedPassword
      }
    });
  }
}
