[%bs.raw {|require("./entryChoice.css")|}];

let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  dashboardPageData: Turaku.dashboardPageData,
  entryMenuData: Turaku.entryMenuData,
  entry: Entry.t,
};

let component = ReasonReact.statelessComponent("EntryChoice");

let isCurrentChoice = ctx =>
  switch (ctx.entryMenuData.entryId) {
  | Some(id) when id == (ctx.entry |> Entry.id) => true
  | Some(_otherId) => false
  | None => false
  };

let containerClasses = ctx => {
  let classes = "mr-3 mt-2 p-2 entry-choice";
  if (ctx |> isCurrentChoice) {
    classes ++ " entry-choice--chosen";
  } else {
    classes;
  };
};

let chooseEntry = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  Js.log("Clicked on event choice with ID: " ++ (ctx.entry |> Entry.id));
  let page =
    Turaku.DashboardPage({
      ...ctx.dashboardPageData,
      menu: Turaku.EntriesMenu({entryId: Some(ctx.entry |> Entry.id)}),
    });
  if (! isCurrentChoice(ctx)) {
    appSend(Turaku.Navigate(SignedInUser({...ctx.userData, page})));
  };
};

let title = (entry: Entry.t) => {
  let trimmedTitle = entry |> Entry.title |> String.trim;
  switch (trimmedTitle) {
  | "" => <em> (str("Nameless entry")) </em>
  | title => <span> (str(title)) </span>
  };
};

let make = (~ctx, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div
      className=(containerClasses(ctx)) onClick=(chooseEntry(ctx, appSend))>
      (ctx.entry |> title)
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