let str = ReasonReact.stringToElement;

type bag = {
  userData: Turaku.userData,
  dashboardPageData: Turaku.dashboardPageData,
  entryMenuData: Turaku.entryMenuData,
  entryId: Entry.id,
};

let component = ReasonReact.statelessComponent("EntryEditor");

let handleTitleChange = _event => ();

let fields = (bag, entry, appSend) =>
  List.map(
    (field: Field.t) =>
      <EntryField
        key=(field |> Field.getKey)
        bag={
          userData: bag.userData,
          dashboardPageData: bag.dashboardPageData,
          entryMenuData: bag.entryMenuData,
          entry,
          field,
        }
        appSend
      />,
    entry |> Entry.getFields,
  );

let make = (~bag, ~appSend, _children) => {
  let entry =
    Turaku.currentEntry(bag.userData, bag.dashboardPageData, bag.entryId);
  {
    ...component,
    render: self =>
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
              fields(bag, entry, appSend)
              |> Array.of_list
              |> ReasonReact.arrayToElement
            )
          </div>
        </div>
        <EntryTags
          bag={
            userData: bag.userData,
            dashboardPageData: bag.dashboardPageData,
            entryMenuData: bag.entryMenuData,
            entry,
          }
          appSend
        />
      </div>,
  };
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