let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("Entry");

let handleTitleChange = (_event) => ();

let fields = (appState, appSend) => {
  /* entry.fields.map.each do |field| */
  /* TODO: The key passed to Field may have to be 'more unique'. */
  <Field
  appState={this.props.appState}
  setAppState={this.props.setAppState}
  field={field}
  key={field.name}
/>
};

/* Is an entry selected? */
let entrySelected = () => true;

let selectedEntry = (appState, appSend) => {
  /* let entry = findEntry(); */

  <div>
  <div className="row">
    <div className="col offset-sm-2">
    <input
      type="text"
      value=entry.title
      onChange=handleTitleChange
      className="my-2"
      placeholder="Entry Title"
    />
  </div>
</div>

<EntryTags appState=appState appSend={appSend} entry={entry}/>
</div>
};

let selectEntryMessage = () => <p>(str("Select an entry, or create a new one."))</p>;

let make = (~appState, ~appSend, _children) => {
  ...component,
  render: (self) => {
    entrySelected() ? selectedEntry() : selectEntryMessage()
  }
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
