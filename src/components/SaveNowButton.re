exception CreateFailure(string);

let str = ReasonReact.string;

type state = {saving: bool};

type action =
  | Save;

let component = ReasonReact.reducerComponent("SaveNowButton");

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

let saveChanges = (team, entry, session, send, appSend) => {
  Js.log("Saving entry: " ++ (entry |> Entry.id));
  send(Save);
  saveChangesInBackground(team, entry, session, appSend);
};

let make = (~session, ~team, ~entry, ~appSend, _children) => {
  ...component,
  willUnmount: ({state}) =>
    if (entry |> Entry.unpersisted && !state.saving) {
      Js.log(
        "Auto-saving (on unmount) entry with ID: " ++ (entry |> Entry.id),
      );
      saveChangesInBackground(team, entry, session, appSend);
    },
  didUpdate: ({newSelf}) => {
    let selectedEntry = team |> Team.entries |> SelectableList.selected;
    if (entry
        |> Entry.unpersisted
        && Some(entry) != selectedEntry
        && !newSelf.state.saving) {
      Js.log("Auto-saving entry with ID: " ++ (entry |> Entry.id));
      saveChanges(team, entry, session, newSelf.send, appSend);
    };
  },
  initialState: () => {saving: false},
  reducer: (action, _state) =>
    switch (action) {
    | Save => ReasonReact.Update({saving: true})
    },
  render: ({state, send}) =>
    <span onClick={_e => saveChanges(team, entry, session, send, appSend)}>
      {
        state.saving ?
          <i className="fas fa-spinner fa-pulse" /> :
          <i className="fas fa-save" />
      }
    </span>,
};