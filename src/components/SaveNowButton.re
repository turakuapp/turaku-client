exception CreateFailure(string);

let str = React.string;

module CreateEntryQuery = [%graphql
  {|
  mutation($teamId: ID!, $iv: String!, $ciphertext: String!, $tagIds: [ID!]!) {
    createEntry(teamId: $teamId, encryptedData: {iv: $iv, ciphertext: $ciphertext}, tagIds: $tagIds) {
      entry {
        id
      }
      errors
    }
  }
  |}
];

module UpdateEntryQuery = [%graphql
  {|
  mutation($id: ID!, $iv: String!, $ciphertext: String!, $tagIds: [ID!]!) {
    updateEntry(id: $id, encryptedData: {iv: $iv, ciphertext: $ciphertext}, tagIds: $tagIds) {
      entry {
        id
      }
      errors
    }
  }
  |}
];

let saveChangesInBackground = (team, entry, session, appSend) => {
  let key = team |> Team.createCryptographicKey;
  entry
  |> Entry.Codec.encode
  |> Js.Json.stringify
  |> EncryptedData.encrypt(key)
  |> Js.Promise.then_(encryptedData =>
       if (entry |> Entry.unsaved) {
         CreateEntryQuery.make(
           ~teamId=team |> Team.id,
           ~iv=
             encryptedData |> EncryptedData.iv |> InitializationVector.toString,
           ~ciphertext=
             encryptedData |> EncryptedData.ciphertext |> CipherText.toString,
           ~tagIds=entry |> Entry.tagIds |> Array.of_list,
           (),
         )
         |> Api.sendAuthenticatedQuery(session)
         |> Js.Promise.then_(response => {
              let updatedEntry = response##createEntry##entry;
              switch (updatedEntry) {
              | Some(e) => entry |> Entry.save(e##id) |> Js.Promise.resolve
              | None => CreateFailure("") |> Js.Promise.reject
              };
            });
       } else {
         UpdateEntryQuery.make(
           ~id=entry |> Entry.id,
           ~iv=
             encryptedData |> EncryptedData.iv |> InitializationVector.toString,
           ~ciphertext=
             encryptedData |> EncryptedData.ciphertext |> CipherText.toString,
           ~tagIds=entry |> Entry.tagIds |> Array.of_list,
           (),
         )
         |> Api.sendAuthenticatedQuery(session)
         |> Js.Promise.then_(_response =>
              entry |> Entry.save(entry |> Entry.id) |> Js.Promise.resolve
            );
       }
     )
  |> Js.Promise.then_(updatedEntry => {
       appSend(Turaku.ReplaceEntry(team |> Team.id, entry, updatedEntry));
       Js.Promise.resolve();
     })
  |> ignore;
};

let saveChanges = (team, entry, session, setSaving, appSend) => {
  Js.log("Saving entry: " ++ (entry |> Entry.id));
  setSaving(_ => true);
  saveChangesInBackground(team, entry, session, appSend);
};

[@react.component]
let make = (~session, ~team, ~entry, ~appSend) => {
  let (saving, setSaving) = React.useState(() => false);

  <span onClick={_e => saveChanges(team, entry, session, setSaving, appSend)}>
    {
      saving ?
        <i className="fas fa-spinner fa-pulse" /> :
        <i className="fas fa-save" />
    }
  </span>;
};