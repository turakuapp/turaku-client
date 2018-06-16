type t = {
  id,
  name: string,
  password: TeamPassword.t,
  entries: list(Entry.t),
  teamMembers: list(TeamMember.t),
  invitations: list(InvitationToUser.t),
}
and id = string;

let name = t => t.name;
let id = t => t.id;
let entries = t => t.entries;
let teamMembers = t => t.teamMembers;
let invitations = t => t.invitations;

let create = (id, name, password) => {
  id,
  name,
  password,
  entries: [],
  teamMembers: [],
  invitations: [],
};

let createCryptographicKey = t =>
  t.password |> CryptographicKey.keyFromTeamPassword;

let replaceEntries = (entries, t) => {...t, entries};
let replaceTeamMembers = (teamMembers, t) => {...t, teamMembers};
let replaceInvitations = (invitations, t) => {...t, invitations};

let addInvitation = (invitation, t) => {
  ...t,
  invitations: [invitation, ...t.invitations],
};

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