import HashService from "./hashService";
import base64js from "base64-js";

export default class CryptoService {
  constructor(key, base64Key = false) {
    this.key = key;
    this.base64Key = base64Key;
  }

  // Promise to encrypt input object with key, with its
  // result being a cryptoObject containing ciphertext and IV.
  async encrypt(object) {
    // Create an initialization vector (IV).
    const iv = new Uint8Array(16);
    window.crypto.getRandomValues(iv);

    // Transform object to an encryptable array.
    const objectArray = this.objectToArrayBuffer(object);

    // Encrypt with crypto key.
    const cryptoKey = await this.cryptoKey();

    const encryptedArrayBuffer = await window.crypto.subtle.encrypt(
      { name: "AES-CBC", iv },
      cryptoKey,
      objectArray
    );

    const bufferBytes = new Uint8Array(encryptedArrayBuffer);

    // Resolve with JSON of object containing encrypted data and IV.
    return {
      ciphertext: base64js.fromByteArray(bufferBytes),
      iv: base64js.fromByteArray(iv)
    };
  }

  // Promise to decrypt cryptoObject with key, and return
  // the originally encrypted object.
  async decrypt(cryptoObject) {
    // Extract IV and cipher array.
    const iv = base64js.toByteArray(cryptoObject.iv);
    const cipherArray = base64js.toByteArray(cryptoObject.ciphertext);

    // Decrypt wth crypto key.
    const cryptoKey = await this.cryptoKey();

    const decryptedArrayBuffer = await window.crypto.subtle.decrypt(
      { name: "AES-CBC", iv },
      cryptoKey,
      cipherArray
    );

    // Resolve with decrypted object.
    return this.arrayBufferToObject(decryptedArrayBuffer);
  }

  arrayBufferToObject(arrayBuffer) {
    const decoder = new TextDecoder("utf-8");
    return JSON.parse(decoder.decode(arrayBuffer));
  }

  objectToArrayBuffer(object) {
    const encoder = new TextEncoder("utf-8");
    return encoder.encode(JSON.stringify(object));
  }

  async cryptoKey() {
    // Crypto-key accepts only 128-bit of 256-bit key data, so depending on the value
    // of boolean base64Key, either hash the key with SHA-256 or treat the key as a
    // Base64 encoded 32-bit string and convert it back to a byte array.
    let keyData = null;

    if (this.base64Key) {
      keyData = base64js.toByteArray(this.key);
    } else {
      keyData = await new HashService(this.key).hash();
    }

    // Create a CryptoKey for AES-CBC with the key hash.
    return await window.crypto.subtle.importKey(
      "raw",
      keyData,
      "AES-CBC",
      false,
      ["encrypt", "decrypt"]
    );
  }
}
