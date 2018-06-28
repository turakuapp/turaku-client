[%bs.raw {|require("./entryChoice.css")|}];

let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  team: Team.t,
  entry: Entry.t,
};

let component = ReasonReact.statelessComponent("EntryChoice");

let isCurrentChoice = ctx =>
  ctx.team |> Team.entries |> SelectableList.selected == Some(ctx.entry);

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
  if (! isCurrentChoice(ctx)) {
    Turaku.SelectEntry(ctx.team, ctx.entry, ctx.userData) |> appSend;
  };
};

let title = entry => {
  let trimmedTitle = entry |> Entry.title |> String.trim;
  switch (trimmedTitle) {
  | "" => <em> (str("Nameless entry")) </em>
  | title => <span> (str(title)) </span>
  };
};

let changesMarker = ctx =>
  if (ctx.entry |> Entry.unpersisted) {
    "(*) " |> str;
  } else {
    ReasonReact.null;
  };

let make = (~ctx, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div
      className=(containerClasses(ctx)) onClick=(chooseEntry(ctx, appSend))>
      (changesMarker(ctx))
      (ctx.entry |> title)
      <SaveNowButton
        ctx={userData: ctx.userData, team: ctx.team, entry: ctx.entry}
        appSend
      />
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