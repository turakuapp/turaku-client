type t = {
  id,
  name: string,
  password: TeamPassword.t,
  entries: list(Entry.t),
}
and id = string;

let getName = t => t.name;

let getId = t => t.id;

let getEntries = t => t.entries;

let create = (id, name, password) => {id, name, password, entries: []};

let getCryptographicKey = t =>
  t.password |> CryptographicKey.keyFromTeamPassword;

let addEntries = (entries, t) => {...t, entries};

let decryptTeams = (response, decryptionKey, encryptedTeams) => {
  let rec aux = (decryptedTeams, teams) =>
    switch (teams) {
    | [team, ...remainingTeams] =>
      let iv =
        EncryptedData.InitializationVector.fromString(
          team##encryptedPassword##iv,
        );
      let ciphertext =
        EncryptedData.CipherText.fromString(
          team##encryptedPassword##ciphertext,
        );
      EncryptedData.create(iv, ciphertext)
      |> EncryptedData.decrypt(decryptionKey)
      |> Js.Promise.then_(plaintext => {
           Js.log(plaintext);
           let teamPassword = plaintext |> TeamPassword.fromString;
           let team = create(team##id, team##name, teamPassword);
           remainingTeams |> aux([team, ...decryptedTeams]);
         });
    | [] => Js.Promise.resolve((response, decryptedTeams))
    };
  encryptedTeams |> Array.to_list |> aux([]);
};