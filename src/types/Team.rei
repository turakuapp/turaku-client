type t;
type id = string;

let create: (id, string, TeamPassword.t) => t;
let createCryptographicKey: t => CryptographicKey.source;

let id: t => id;
let name: t => string;
let password: t => TeamPassword.t;
let entries: t => SelectableList.t(Entry.t);
let tags: t => SelectableList.t(Tag.t);
let teamMembers: t => SelectableList.t(TeamMember.t);
let invitations: t => SelectableList.t(InvitationToUser.t);

let replaceEntries: (SelectableList.t(Entry.t), t) => t;
let replaceTags: (SelectableList.t(Tag.t), t) => t;
let replaceTeamMembers: (SelectableList.t(TeamMember.t), t) => t;
let replaceInvitations: (SelectableList.t(InvitationToUser.t), t) => t;

let addInvitation: (InvitationToUser.t, t) => t;
let removeInvitation: (InvitationToUser.t, t) => t;

let decryptTeams:
  (
    'a,
    CryptographicKey.source,
    array(
      {
        ..
        "encryptedPassword": {
          ..
          "ciphertext": string,
          "iv": string,
        },
        "id": id,
        "name": string,
      },
    )
  ) =>
  Js.Promise.t(('a, list(t)));