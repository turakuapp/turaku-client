type t;
type id = string;

let create: (id, string, TeamPassword.t) => t;
let createCryptographicKey: t => CryptographicKey.source;

let name: t => string;
let id: t => string;
let entries: t => list(Entry.t);
let teamMembers: t => list(TeamMember.t);
let invitations: t => list(InvitationToUser.t);

let replaceEntries: (list(Entry.t), t) => t;
let replaceTeamMembers: (list(TeamMember.t), t) => t;
let replaceInvitations: (list(InvitationToUser.t), t) => t;

let addInvitation: (InvitationToUser.t, t) => t;

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