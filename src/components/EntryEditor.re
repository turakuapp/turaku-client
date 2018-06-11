let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("EntryEditor");

let handleTitleChange = _event => ();

let fields = (appState, appSend, entry: Entry.t) =>
  List.map(
    (field: Field.t) => <EntryField appState appSend field />,
    entry |> Entry.getFields,
  );

/* Is an entry selected? */
let entrySelected = () => false;

let selectedEntry = (appState: Turaku.state) =>
  /* TODO: This should actually return one of unsavedEntries, or entries. */
  switch (appState.currentPage) {
  | DashboardPage(_, EntrySelected(id)) =>
    appState.entries |> List.find((e: Entry.t) => e |> Entry.getId == id)
  | _ => failwith("Entry.selectedEntry called when no entry was selected!")
  };

let selectedEntry = (appState, appSend) => {
  let entry = selectedEntry(appState);
  <div>
    <div className="row">
      <div className="col offset-sm-2">
        <input
          _type="text"
          value=(entry |> Entry.getTitle)
          onChange=handleTitleChange
          className="my-2"
          placeholder="Entry Title"
        />
        (
          fields(appState, appSend, entry)
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </div>
    </div>
    <EntryTags appState appSend entry />
  </div>;
};

let selectEntryMessage = () =>
  <p> (str("Select an entry, or create a new one.")) </p>;

let make = (~appState, ~appSend, _children) => {
  ...component,
  render: self =>
    entrySelected() ? selectedEntry(appState, appSend) : selectEntryMessage(),
};
/* export default class Entry extends React.Component {
     constructor(props) {
       super(props);

       this.handleTitleChange = this.handleTitleChange.bind(this);
     }

     entrySelected() {
       return _.isString(this.props.appState.entryId);
     }

     handleTitleChange(event) {
       console.log("Changing title to " + event.target.value);

       const entryId = this.props.appState.entryId;
       const unsavedEntries = _.cloneDeep(this.props.appState.unsavedEntries);
       let unsavedEntry = unsavedEntries[entryId];

       if (!_.isObject(unsavedEntry)) {
         // The unsaved entry doesn't exist, copy it into the clone
         // of unsaved entries list from list of saved entries.
         unsavedEntry = _.cloneDeep(this.props.appState.entries[entryId]);
         unsavedEntries[entryId] = unsavedEntry;
       }

       unsavedEntry.title = event.target.value;
       this.props.setAppState({ unsavedEntries: unsavedEntries });
     }

     findEntry() {
       const entryId = this.props.appState.entryId;

       if (_.startsWith(entryId, "N")) {
         return this.props.appState.unsavedEntries[entryId];
       } else {
         const unsavedEntry = this.props.appState.unsavedEntries[entryId];
         if (_.isObject(unsavedEntry)) {
           return unsavedEntry;
         } else {
           return this.props.appState.entries[entryId];
         }
       }
     }

     render() {
       return (
         <div>
           !!! EXTRACTED !!!
         </div>
       );
     }
   } */