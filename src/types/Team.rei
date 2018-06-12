type t;

type id = string;

let getName: t => string;

let getId: t => string;

let getEntries: t => list(Entry.t);

let create: (id, string, TeamPassword.t) => t;

let addEntries: (list(Entry.t), t) => t;

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

let getCryptographicKey: t => CryptographicKey.source;