exception LoadEntriesFailure;

let handleLoadEntriesFailure = () =>
  [@bs.open]
  (
    fun
    | LoadEntriesFailure => {
        Webapi.Dom.window
        |> Webapi.Dom.Window.alert(
             "It looks like you have lost access to the current team. Reloading Turaku...",
           );
        Webapi.Dom.location |> Webapi.Dom.Location.reload;
        Js.Promise.resolve();
      }
  );

let str = React.string;

let addEntry = (appSend, _event) => {
  Js.log("Add a new entry");
  appSend(Turaku.AddNewEntry);
};

let entryChoices = (session, team, search, appSend) =>
  team
  |> Team.entries
  |> SelectableList.all
  |> List.filter(entry =>
       if (entry |> Entry.unpersisted) {
         true;
       } else {
         let searchExp = search |> Js.Re.fromStringWithFlags(~flags="i");
         entry |> Entry.title |> Js.String.search(searchExp) > (-1);
       }
     )
  |> List.map(entry =>
       <EntryChoice key={entry |> Entry.id} session team entry appSend />
     )
  |> Array.of_list;

module EntriesQuery = [%graphql
  {|
  query($teamId: ID!) {
    team(id: $teamId) {
      tags {
        id
        encryptedName {
          iv
          ciphertext
        }
        nameHash
      }
      entries {
        id
        encryptedData {
          iv
          ciphertext
        }
        tagIds
      }
    }
  }
  |}
];

let decryptEntries = (decryptionKey, encryptedEntries) => {
  let rec aux = (decryptedEntries, entries) =>
    switch (entries) {
    | [entry, ...remainingEntries] =>
      let iv = InitializationVector.fromString(entry##encryptedData##iv);
      let ciphertext =
        CipherText.fromString(entry##encryptedData##ciphertext);
      EncryptedData.create(iv, ciphertext)
      |> EncryptedData.decrypt(decryptionKey)
      |> Js.Promise.then_(plaintext => {
           Js.log(plaintext);
           remainingEntries
           |> aux([
                plaintext
                |> Json.parseOrRaise
                |> Entry.Codec.decode(
                     entry##id,
                     entry##tagIds |> Array.to_list,
                   ),
                ...decryptedEntries,
              ]);
         });
    | [] => decryptedEntries |> Js.Promise.resolve
    };
  encryptedEntries |> Array.to_list |> aux([]);
};

let decryptTags = (decryptionKey, encryptedTags) => {
  let rec aux = (decryptedTags, tags) =>
    switch (tags) {
    | [tag, ...remainingTags] =>
      let iv = InitializationVector.fromString(tag##encryptedName##iv);
      let ciphertext = CipherText.fromString(tag##encryptedName##ciphertext);
      EncryptedData.create(iv, ciphertext)
      |> EncryptedData.decrypt(decryptionKey)
      |> Js.Promise.then_(plaintext => {
           Js.log(plaintext);
           let tag =
             Tag.create(tag##id, ~name=plaintext, ~nameHash=tag##nameHash);
           remainingTags |> aux([tag, ...decryptedTags]);
         });
    | [] => decryptedTags |> Js.Promise.resolve
    };
  encryptedTags |> Array.to_list |> aux([]);
};

let loadEntries = (session, team, appSend) =>
  EntriesQuery.make(~teamId=team |> Team.id, ())
  |> Api.sendAuthenticatedQuery(session)
  |> Js.Promise.then_(response =>
       switch (response##team) {
       | Some(responseTeam) =>
         let decryptionKey = team |> Team.createCryptographicKey;

         Js.Promise.all2((
           responseTeam##entries |> decryptEntries(decryptionKey),
           responseTeam##tags |> decryptTags(decryptionKey),
         ));
       | None => Js.Promise.reject(LoadEntriesFailure)
       }
     )
  |> Js.Promise.then_(((entries, tags)) => {
       Js.log(
         "Loaded "
         ++ (entries |> List.length |> string_of_int)
         ++ " entries and "
         ++ (tags |> List.length |> string_of_int)
         ++ " tags!",
       );

       appSend(Turaku.RefreshEntries(team |> Team.id, entries, tags));
       Js.Promise.resolve();
     })
  |> Js.Promise.catch(error =>
       switch (error |> handleLoadEntriesFailure()) {
       | Some(x) => x
       | None =>
         Js.log(error);
         Js.Promise.resolve();
       }
     )
  |> ignore;

let getSelection = (session, team, appSend, entry) =>
  switch (entry) {
  | Some(entry) => <EntryEditor session team entry appSend />
  | None =>
    <p className="m-2"> {str("Select an entry, or create a new one.")} </p>
  };

let updateSearch = (setSearch, _event) =>
  setSearch(_ => DomUtils.getValueOfInputById("sign-in-menu__search"));

[@react.component]
let make = (~session, ~team, ~appSend) => {
  let (search, setSearch) = React.useState(() => "");

  React.useEffect1(
    () => {
      loadEntries(session, team, appSend);
      None;
    },
    [|team |> Team.id|],
  );

  <div className="flex">
    <div className="w-1/5 flex flex-col h-screen">
      <div className="mt-2 flex flex-no-shrink flex-row mx-2">
        <input
          id="sign-in-menu__search"
          type_="text"
          onChange={updateSearch(setSearch)}
          placeholder="Search"
          className="flex-shrink pl-2 rounded min-w-0"
          value=search
        />
        <button
          className="flex-no-shrink ml-2 btn btn-blue"
          onClick={addEntry(appSend)}>
          {"+" |> str}
        </button>
      </div>
      <div className="overflow-scroll mt-2">
        {entryChoices(session, team, search, appSend) |> React.array}
      </div>
    </div>
    <div className="w-4/5 bg-white">
      {
        team
        |> Team.entries
        |> SelectableList.selected
        |> getSelection(session, team, appSend)
      }
    </div>
  </div>;
};