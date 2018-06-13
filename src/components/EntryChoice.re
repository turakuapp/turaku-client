[%bs.raw {|require("./entryChoice.css")|}];

let str = ReasonReact.stringToElement;

type bag = {
  userData: Turaku.userData,
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
  let page =
    Turaku.DashboardPage({
      ...bag.dashboardPageData,
      menu: Turaku.EntriesMenu({entryId: Some(bag.entry |> Entry.getId)}),
    });
  if (! isCurrentChoice(bag)) {
    appSend(Turaku.Navigate(SignedInUser({...bag.userData, page})));
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
  render: _self =>
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