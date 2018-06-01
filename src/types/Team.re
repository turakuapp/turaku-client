type t = {
  id,
  name: string,
  encryptedPassword: EncryptedData.t,
}
and id = string;

let name = team => team.name;

let create = (id, name, encryptedPassword) => {id, name, encryptedPassword};