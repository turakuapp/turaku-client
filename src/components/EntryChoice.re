[%bs.raw {|require("./entryChoice.css")|}];

let str = ReasonReact.stringToElement;

type bag = {
  signedInData: Turaku.signedInData,
  dashboardPageData: Turaku.dashboardPageData,
  entryMenuData: Turaku.entryMenuData,
  entry: Entry.t,
};

let component = ReasonReact.statelessComponent("EntryChoice");

let isCurrentChoice = bag =>
  switch (bag.entryMenuData.entryId) {
  | Some(id) when id == (bag.entry |> Entry.getId) => true
  | Some(_otherId) => false
  | None => false
  };

let containerClasses = bag => {
  let classes = "mr-3 mt-2 p-2 entry-choice";
  if (bag |> isCurrentChoice) {
    classes ++ " entry-choice--chosen";
  } else {
    classes;
  };
};

let chooseEntry = (bag, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  Js.log("Clicked on event choice with ID: " ++ (bag.entry |> Entry.getId));
  let updatedPage =
    Turaku.DashboardPage({
      ...bag.dashboardPageData,
      dashboardMenu:
        Turaku.EntriesMenu({entryId: Some(bag.entry |> Entry.getId)}),
    });
  if (! isCurrentChoice(bag)) {
    appSend(Turaku.Navigate(SignedIn(updatedPage, bag.signedInData)));
  };
};

let title = (entry: Entry.t) => {
  let trimmedTitle = entry |> Entry.getTitle |> String.trim;
  switch (trimmedTitle) {
  | "" => <em> (str("Nameless entry")) </em>
  | title => <span> (str(title)) </span>
  };
};

let make = (~bag, ~appSend, _children) => {
  ...component,
  render: self =>
    <div
      className=(containerClasses(bag)) onClick=(chooseEntry(bag, appSend))>
      (bag.entry |> title)
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