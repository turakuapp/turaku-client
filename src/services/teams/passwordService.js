import RandomService from "../randomService";
import HashService from "../hashService";
import base64js from "base64-js";

export default class PasswordService {
  newPassword() {
    return new RandomService().string();
  }

  async updatePassword(alteredPassword) {
    const updatedPasswordBuffer = await new HashService(alteredPassword).hash();
    const updatedPasswordBytes = new Uint8Array(updatedPasswordBuffer);
    return base64js.fromByteArray(updatedPasswordBytes);
  }
}
