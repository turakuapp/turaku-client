let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  team: Team.t,
};

type action =
  | UpdateSearch(string);

type state = {search: string};

let component = ReasonReact.reducerComponent("Entries");

let addEntry = (ctx, appSend, _event) => {
  Js.log("Add a new entry");
  appSend(Turaku.AddNewEntry);
};

let entryChoices = (ctx, state, appSend) =>
  ctx.team
  |> Team.entries
  |> SelectableList.all
  |> List.filter(entry =>
       if (entry |> Entry.unpersisted) {
         true;
       } else {
         let searchExp =
           state.search |> Js.Re.fromStringWithFlags(~flags="i");
         entry |> Entry.title |> Js.String.search(searchExp) > (-1);
       }
     )
  |> List.map(entry =>
       <EntryChoice
         key=(entry |> Entry.id)
         ctx={userData: ctx.userData, team: ctx.team, entry}
         appSend
       />
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

let loadEntries = (ctx, appSend) =>
  EntriesQuery.make(~teamId=ctx.team |> Team.id, ())
  |> Api.sendAuthenticatedQuery(ctx.userData.session)
  |> Js.Promise.then_(response => {
       let decryptionKey = ctx.team |> Team.createCryptographicKey;
       let team = response##team;

       Js.Promise.all2((
         team##entries |> decryptEntries(decryptionKey),
         team##tags |> decryptTags(decryptionKey),
       ));
     })
  |> Js.Promise.then_(((entries, tags)) => {
       Js.log(
         "Loaded "
         ++ (entries |> List.length |> string_of_int)
         ++ " entries and "
         ++ (tags |> List.length |> string_of_int)
         ++ " tags!",
       );

       appSend(Turaku.RefreshEntries(ctx.team |> Team.id, entries, tags));
       Js.Promise.resolve();
     })
  |> ignore;

let getSelection = (ctx, appSend, entry) =>
  switch (entry) {
  | Some(entry) =>
    <EntryEditor
      ctx={userData: ctx.userData, team: ctx.team, entry}
      appSend
    />
  | None => <p> (str("Select an entry, or create a new one.")) </p>
  };

let updateSearch = (send, _event) => {
  let searchString = DomUtils.getValueOfInputById("sign-in-menu__search");
  send(UpdateSearch(searchString));
};

let make = (~ctx, ~appSend, _children) => {
  ...component,
  initialState: () => {search: ""},
  reducer: (action, _state) =>
    switch (action) {
    | UpdateSearch(search) => ReasonReact.Update({search: search})
    },
  didMount: _self => loadEntries(ctx, appSend),
  render: ({state, send}) =>
    <div className="flex">
      <div className="w-1/5 flex flex-col h-screen">
        <div className="py-2 flex flex-no-shrink flex-col">
          <input
            id="sign-in-menu__search"
            _type="text"
            onChange=(updateSearch(send))
            placeholder="Search"
            className="p-2 rounded mx-2"
          />
          <button
            className="btn btn-blue mt-2 mx-2"
            onClick=(addEntry(ctx, appSend))>
            (str("Add a new entry"))
          </button>
        </div>
        <div className="overflow-scroll">
          (entryChoices(ctx, state, appSend) |> ReasonReact.array)
        </div>
      </div>
      <div className="w-4/5 bg-grey-lighter">
        (
          ctx.team
          |> Team.entries
          |> SelectableList.selected
          |> getSelection(ctx, appSend)
        )
      </div>
    </div>,
};