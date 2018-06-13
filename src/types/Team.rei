type t;

type id = string;

let name: t => string;

let id: t => string;

let entries: t => list(Entry.t);

let teamMembers: t => list(TeamMember.t);

let create: (id, string, TeamPassword.t) => t;

let addEntries: (list(Entry.t), t) => t;

let addTeamMembers: (list(TeamMember.t), t) => t;

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

let createCryptographicKey: t => CryptographicKey.source;