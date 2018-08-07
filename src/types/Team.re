type t = {
  id,
  name: string,
  password: TeamPassword.t,
  entries: SelectableList.t(Entry.t),
  unsavedEntries: SelectableList.t(UnsavedEntry.t),
  teamMembers: list(TeamMember.t),
  invitations: list(InvitationToUser.t),
  tags: SelectableList.t(Tag.t),
}
and id = string;

type ts = {
  selected: t,
  rest: list(t),
};

let selected = ts => ts.selected;

let addItem = (i, ts) => {selected: i, rest: [ts.selected, ...ts.rest]};

let name = t => t.name;
let id = t => t.id;
let password = t => t.password;
let entries = t => t.entries;
let tags = t => t.tags;
let teamMembers = t => t.teamMembers;
let invitations = t => t.invitations;

let create = (id, name, password) => {
  id,
  name,
  password,
  entries: SelectableList.empty(),
  unsavedEntries: SelectableList.empty(),
  teamMembers: [],
  invitations: [],
  tags: SelectableList.empty(),
};

let createCryptographicKey = t =>
  t.password |> CryptographicKey.keyFromTeamPassword;

let replaceEntries = (entries, t) => {...t, entries};
let replaceTags = (tags, t) => {...t, tags};
let replaceTeamMembers = (teamMembers, t) => {...t, teamMembers};
let replaceInvitations = (invitations, t) => {...t, invitations};

let addInvitation = (invitation, t) => {
  ...t,
  invitations: [invitation, ...t.invitations],
};

let removeInvitation = (invitation, t) => {
  ...t,
  invitations: t.invitations |> List.filter(i => i != invitation),
};

/**
 * TODO: This decrypts teams one-by-one, which is slow. Dispatch all decrypt
 * promises, and resolve them together with Js.Promise.all. */
let decryptTeams = (response, decryptionKey, encryptedTeams) => {
  let rec aux = (decryptedTeams, teams) =>
    switch (teams) {
    | [team, ...remainingTeams] =>
      let iv = InitializationVector.fromString(team##encryptedPassword##iv);
      let ciphertext =
        CipherText.fromString(team##encryptedPassword##ciphertext);
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