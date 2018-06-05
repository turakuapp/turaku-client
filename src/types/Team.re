type t = {
  id,
  name: string,
  encryptedPassword: EncryptedData.t,
}
and id = string;

let getName = t => t.name;

let getId = t => t.id;

let create = (id, name, encryptedPassword) => {id, name, encryptedPassword};