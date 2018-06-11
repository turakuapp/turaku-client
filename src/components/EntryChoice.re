[%bs.raw {|require("./entryChoice.css")|}];

let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("EntryChoice");

let containerClasses = (appState, entry) => {
  let classes = "mr-3 mt-2 p-2 entry-choice";
  /* if (entry == appState.focus) {
       classes ++ " entry-choice--chosen";
     } else {
       classes;
     } */
  classes;
};

let chooseEntry = (appState, appSend, entry, event) => {
  /* if (this.props.entryId !== this.props.appState.entryId) {
       this.props.setAppState({ entryId: this.props.entryId });
     } */
  event |> DomUtils.preventMouseEventDefault;
  let team =
    switch (appState.Turaku.currentPage) {
    | DashboardPage(selectedTeam, _) => selectedTeam
    | _ => failwith("EntryChoice.chooseEntry called without a selected team!")
    };
  appSend(
    Turaku.Navigate(
      DashboardPage(team, Turaku.EntriesMenu(Turaku.EntrySelected(entry))),
    ),
  );
  Js.log("Clicked on event choice with ID: " ++ (entry |> Entry.getId));
};

let title = (entry: Entry.t) => {
  let trimmedTitle = entry |> Entry.getTitle |> String.trim;
  switch (trimmedTitle) {
  | "" => <em> (str("Nameless entry")) </em>
  | title => <span> (str(title)) </span>
  };
};

let make = (~appState, ~appSend, ~entry, _children) => {
  ...component,
  render: self =>
    <div
      className=(containerClasses(appState, entry))
      onClick=(chooseEntry(appState, appSend, entry))>
      (entry |> title)
    </div>,
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