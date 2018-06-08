type t = {
  id,
  name: string,
  encryptedPassword: EncryptedData.t,
}
and id = string;

let getName = t => t.name;

let getId = t => t.id;

let getEncryptedPassword = t => t.encryptedPassword;

let create = (id, name, encryptedPassword) => {id, name, encryptedPassword};