type t;
type id = string;

let create: (id, string, TeamPassword.t) => t;
let createCryptographicKey: t => CryptographicKey.source;

let id: t => id;
let name: t => string;
let password: t => TeamPassword.t;
let entries: t => SelectableList.t(Entry.t);
let tags: t => SelectableList.t(Tag.t);
let teamMembers: t => list(TeamMember.t);
let invitations: t => list(InvitationToUser.t);

let replaceEntries: (SelectableList.t(Entry.t), t) => t;
let replaceTags: (SelectableList.t(Tag.t), t) => t;
let replaceTeamMembers: (list(TeamMember.t), t) => t;
let replaceInvitations: (list(InvitationToUser.t), t) => t;

let addInvitation: (InvitationToUser.t, t) => t;
let removeInvitation: (InvitationToUser.t, t) => t;

let decryptTeams:
  (
    CryptographicKey.source,
    array({
      ..
      "encryptedPassword": {
        ..
        "ciphertext": string,
        "iv": string,
      },
      "id": id,
      "name": string,
    })
  ) =>
  Js.Promise.t(list(t));