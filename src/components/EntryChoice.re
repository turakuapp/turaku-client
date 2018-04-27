let str = ReasonReact.stringToElement;

let component  = ReasonReact.statelessComponent("EntryChoice");

let containerClasses = (appState, entry) => {
  let classes = "mr-3 mt-2 p-2 entry-choice";

  /* if (entry == appState.focus) {
    classes ++ " entry-choice--chosen";
  } else {
    classes;
  } */

  classes
};

let chooseEntry = (appState, appSend, entry, _event) => {
  /* if (this.props.entryId !== this.props.appState.entryId) {
    this.props.setAppState({ entryId: this.props.entryId });
  } */
  ();
};

let title = (entry: Entry.t) => {
  let trimmedTitle = entry.title |> String.trim;

  switch(trimmedTitle) {
  | "" => <em>(str("Nameless entry"))</em>
  | title => <span>(str(title))</span>
  };
};

let make = (~appState, ~appSend, ~entry: Entry.t, _children) => {
  ...component,
  render: (self) => {
    <div className=containerClasses(appState, entry) onClick=chooseEntry(appState, appSend, entry)>
      (entry |> title)
    </div>
  }
};

/* export default class EntryChoice extends React.Component {
  entry() {
    if (this.props.appState.unsavedEntries.hasOwnProperty(this.props.entryId)) {
      return this.props.appState.unsavedEntries[this.props.entryId];
    } else {
      return this.props.appState.entries[this.props.entryId];
    }
  }
}*/