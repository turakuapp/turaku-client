[%bs.raw {|require("./entries.css")|}];

let str = ReasonReact.stringToElement;

type bag = {
  userData: Turaku.userData,
  dashboardPageData: Turaku.dashboardPageData,
  entryMenuData: Turaku.entryMenuData,
};

let component = ReasonReact.statelessComponent("Entries");

let addEntry = _event => Js.log("Add an entry, maybe?");

let entryChoices = (bag, appSend) => {
  let team = Turaku.currentTeam(bag.userData, bag.dashboardPageData);
  team
  |> Team.entries
  |> List.map(entry =>
       <EntryChoice
         key=(entry |> Entry.id)
         bag={
           userData: bag.userData,
           dashboardPageData: bag.dashboardPageData,
           entryMenuData: bag.entryMenuData,
           entry,
         }
         appSend
       />
     )
  |> Array.of_list;
};

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
      let iv =
        EncryptedData.InitializationVector.fromString(
          entry##encryptedData##iv,
        );
      let ciphertext =
        EncryptedData.CipherText.fromString(entry##encryptedData##ciphertext);
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

let loadEntries = (bag, appSend) => {
  let selectedTeam = Turaku.currentTeam(bag.userData, bag.dashboardPageData);
  EntriesQuery.make(~teamId=selectedTeam |> Team.id, ())
  |> Api.sendAuthenticatedQuery(bag.userData.session)
  |> Js.Promise.then_(response => {
       Js.log(
         "Loaded entries! Count: "
         ++ (response##team##entries |> Array.length |> string_of_int),
       );
       let decryptionKey = selectedTeam |> Team.createCryptographicKey;
       response##team##entries |> decryptEntries(decryptionKey);
     })
  |> Js.Promise.then_(decryptedEntries => {
       appSend(
         Turaku.RefreshEntries(
           selectedTeam |> Team.id,
           decryptedEntries,
           bag.userData,
         ),
       );
       Js.Promise.resolve();
     })
  |> ignore;
};

let getSelection = (bag, appSend, entryId) =>
  switch (entryId) {
  | Some(id) =>
    <EntryEditor
      bag={
        userData: bag.userData,
        dashboardPageData: bag.dashboardPageData,
        entryMenuData: bag.entryMenuData,
        entryId: id,
      }
      appSend
    />
  | None => <p> (str("Select an entry, or create a new one.")) </p>
  };

let make = (~bag, ~appSend, _children) => {
  ...component,
  didMount: _self => loadEntries(bag, appSend),
  render: _self =>
    <div className="row">
      <div className="col-3">
        <div className="entries__nav">
          <div className="pt-2">
            <input _type="text" placeholder="Search" className="mr-2" />
            <button className="btn btn-primary btn-sm" onClick=addEntry>
              (str("Add new"))
            </button>
          </div>
          (entryChoices(bag, appSend) |> ReasonReact.arrayToElement)
        </div>
      </div>
      <div className="col entry-editor__container">
        (bag.entryMenuData.entryId |> getSelection(bag, appSend))
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