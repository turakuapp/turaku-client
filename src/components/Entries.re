[%bs.raw {|require("./entries.css")|}];

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
  appSend(Turaku.AddNewEntry(ctx.team, ctx.userData));
};

let entryChoices = (ctx, state, appSend) =>
  ctx.team
  |> Team.entries
  |> SelectableList.all
  |> List.filter(entry => {
       let searchExp = state.search |> Js.Re.fromStringWithFlags(~flags="i");
       entry |> Entry.title |> Js.String.search(searchExp) > (-1);
     })
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
      entries {
        id
        encryptedData {
          iv
          ciphertext
        }
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
                |> Entry.Codec.decode(entry##id),
                ...decryptedEntries,
              ]);
         });
    | [] => decryptedEntries |> Js.Promise.resolve
    };
  encryptedEntries |> Array.to_list |> aux([]);
};

let loadEntries = (ctx, appSend) =>
  EntriesQuery.make(~teamId=ctx.team |> Team.id, ())
  |> Api.sendAuthenticatedQuery(ctx.userData.session)
  |> Js.Promise.then_(response => {
       Js.log(
         "Loaded entries! Count: "
         ++ (response##team##entries |> Array.length |> string_of_int),
       );
       let decryptionKey = ctx.team |> Team.createCryptographicKey;
       response##team##entries |> decryptEntries(decryptionKey);
     })
  |> Js.Promise.then_(decryptedEntries => {
       appSend(
         Turaku.RefreshEntries(ctx.team, decryptedEntries, ctx.userData),
       );
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
    <div className="row">
      <div className="col-3">
        <div className="entries__search mt-2">
          <input
            id="sign-in-menu__search"
            _type="text"
            onChange=(updateSearch(send))
            placeholder="Search"
            className="mr-2"
          />
          <button
            className="btn btn-primary btn-sm"
            onClick=(addEntry(ctx, appSend))>
            (str("Add new"))
          </button>
        </div>
        <div className="entries__nav">
          <div className="entries__scroll">
            (entryChoices(ctx, state, appSend) |> ReasonReact.array)
          </div>
        </div>
      </div>
      <div className="col entries__editor-container">
        (
          ctx.team
          |> Team.entries
          |> SelectableList.selected
          |> getSelection(ctx, appSend)
        )
      </div>
    </div>,
};
/* export default class Entries extends React.Component {
     constructor(props) {
       super(props);
       this.addEntry = this.addEntry.bind(this);
     }

     componentDidMount() {
       // Load entries.
       !!! EXTRACTED !!!
       }
     }

     addEntry() {
       console.log("Adding a new entry...");

       const entry = {
         title: "",
         persisted: false,
         fields: [
           { name: "user", type: "user", value: "" },
           { name: "password", type: "password", value: "" }
         ],
         tags: []
       };

       let unsavedEntries = {};
       let newKey = null;

       if (_.isObject(this.props.appState.unsavedEntries)) {
         console.log("Stale entries exist...");
         unsavedEntries = _.cloneDeep(this.props.appState.unsavedEntries);

         const newEntryKeys = _.filter(Object.keys(unsavedEntries), key => {
           return _.startsWith(key, "N");
         });

         console.log("New entry keys are: ", newEntryKeys);

         if (_.isEmpty(newEntryKeys)) {
           newKey = "N1";
         } else {
           const newKeyInteger =
             parseInt(
               _.max(
                 _.map(newEntryKeys, key => {
                   return _.join(_.slice(key, 1));
                 })
               ),
               10
             ) + 1;

           newKey = "N" + newKeyInteger;
         }
       } else {
         console.log("Stale entries do not exist...");
         newKey = "N1";
       }

       unsavedEntries[newKey] = entry;

       this.props.setAppState({
         entryId: newKey,
         unsavedEntries: unsavedEntries
       });
     }

     entryChoices() {
       const choices = [];

       // List all unpersisted entries.
       _.forOwn(this.props.appState.unsavedEntries, (unsavedEntry, entryId) => {
         if (unsavedEntry.persisted === false) {
           choices.push(
             <EntryChoice
               key={entryId}
               appState={this.props.appState}
               setAppState={this.props.setAppState}
               entryId={entryId}
               entry={unsavedEntry}
             />
           );
         }
       });

       // List all persisted entries.
       _.forOwn(this.props.appState.entries, (entry, entryId) => {
         choices.push(
           <EntryChoice
             key={entryId}
             appState={this.props.appState}
             setAppState={this.props.setAppState}
             entryId={entryId}
             entry={entry}
           />
         );
       });

       return choices;
     }

     render() {
       return (
         !!! EXTRACTED !!!
       );
     }
   } */