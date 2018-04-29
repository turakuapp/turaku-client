/* import "./entries.css"; */
let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("Entries");

let addEntry = _event => Js.log("Add an entry, maybe?");

let entryChoices = () => [|/* TODO: Actually write this, maybe? */|];

let make = (~appState, ~appSend, _children) => {
  ...component,
  render: self =>
    <div className="row">
      <div className="col-3">
        <div className="entries__nav">
          <div className="pt-2">
            <input _type="text" placeholder="Search" className="mr-2" />
            <button className="btn btn-primary btn-sm" onClick=addEntry>
              (str("Add new"))
            </button>
          </div>
          (entryChoices() |> ReasonReact.arrayToElement)
        </div>
      </div>
      <div className="col-8"> <EntryEditor appState appSend /> </div>
    </div>,
};
/* export default class Entries extends React.Component {
     constructor(props) {
       super(props);
       this.addEntry = this.addEntry.bind(this);
     }

     componentDidMount() {
       // Load entries.
       if (_.isEmpty(this.props.appState.entries)) {
         const listService = new ListEntriesService(
           this.props.appState.token,
           this.props.appState.encryptionHash,
           this.props.appState.team.id,
           this.props.appState,
           this.props.setAppState
         );

         listService.list().then(entries => {
           for (const encryptedEntry of entries) {
             this.decryptEntry(encryptedEntry);
           }
         });
       }
     }

     async decryptEntry(encryptedEntry) {
       const crypto = new CryptoService(this.props.appState.team.password, true);
       const decryptedEntry = await crypto.decrypt(encryptedEntry.encrypted_data);

       // Tag IDs loaded from server are not encrypted. Attach them directly to decrypted entry.
       decryptedEntry.tags = encryptedEntry.tags;

       // Mark all loaded entries as persisted, so that changes
       // to them are handled as updates instead of creates.
       decryptedEntry.persisted = true;

       const entriesClone = _.cloneDeep(this.props.appState.entries);
       entriesClone[encryptedEntry.id] = decryptedEntry;
       this.props.setAppState({ entries: entriesClone });
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