let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  entry: Entry.t,
};

let component = ReasonReact.statelessComponent("EntryEditor");

let handleTitleChange = _event => ();

let fields = (ctx, appSend) =>
  List.map(
    (field: Field.t) =>
      <EntryField
        key=(field |> Field.getKey)
        ctx={userData: ctx.userData, entry: ctx.entry, field}
        appSend
      />,
    ctx.entry |> Entry.fields,
  );

let make = (~ctx, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div>
      <div className="row">
        <div className="col offset-sm-2">
          <input
            _type="text"
            value=(ctx.entry |> Entry.title)
            onChange=handleTitleChange
            className="my-2"
            placeholder="Entry Title"
          />
          (fields(ctx, appSend) |> Array.of_list |> ReasonReact.array)
        </div>
      </div>
      <EntryTags ctx={userData: ctx.userData, entry: ctx.entry} appSend />
    </div>,
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