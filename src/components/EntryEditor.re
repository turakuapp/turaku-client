let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  team: Team.t,
  entry: Entry.t,
};

let component = ReasonReact.statelessComponent("EntryEditor");

let handleTitleChange = (ctx, appSend, _event) => {
  Js.log("Editing title");
  let title = DomUtils.getValueOfInputById("entry-editor__title");
  appSend(Turaku.EditEntryTitle(ctx.team, ctx.entry, title, ctx.userData));
};

let fields = (ctx, appSend) => {
  let index = ref(-1);
  Array.map(
    (field: Field.t) => {
      index := index^ + 1;
      <EntryField
        key=(field |> Field.key)
        ctx={
          userData: ctx.userData,
          team: ctx.team,
          entry: ctx.entry,
          field,
          index: index^,
        }
        appSend
      />;
    },
    ctx.entry |> Entry.fields,
  );
};

let make = (~ctx, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div>
      <div className="row">
        <div className="col offset-sm-2">
          <input
            _type="text"
            value=(ctx.entry |> Entry.title)
            onChange=(handleTitleChange(ctx, appSend))
            className="my-2"
            placeholder="Entry Title"
            id="entry-editor__title"
          />
        </div>
      </div>
      (fields(ctx, appSend) |> ReasonReact.array)
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