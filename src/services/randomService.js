import base64js from "base64-js";

export default class RandomService {
  string() {
    return base64js.fromByteArray(this.array());
  }

  array() {
    const buffer = new Uint8Array(32);
    window.crypto.getRandomValues(buffer);
    return buffer;
  }
}
