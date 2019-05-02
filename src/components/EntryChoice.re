let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("EntryChoice");

let isCurrentChoice = (team, entry) =>
  team |> Team.entries |> SelectableList.selected == Some(entry);

let containerClasses = (team, entry) => {
  let classes = "cursor-pointer p-2 font-thin hover:bg-white flex justify-between items-center";
  if (isCurrentChoice(team, entry)) {
    classes ++ " bg-white font-normal";
  } else {
    classes;
  };
};

let chooseEntry = (team, entry, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  Js.log("Clicked on event choice with ID: " ++ (entry |> Entry.id));
  if (!isCurrentChoice(team, entry)) {
    Turaku.SelectEntry(entry) |> appSend;
  };
};

let title = entry => {
  let trimmedTitle = entry |> Entry.title |> String.trim;
  switch (trimmedTitle) {
  | "" => <em> {str("Nameless entry")} </em>
  | title => <span> {str(title)} </span>
  };
};

let make = (~session, ~team, ~entry, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div
      className={containerClasses(team, entry)}
      onClick={chooseEntry(team, entry, appSend)}>
      {entry |> title}
      {
        if (entry |> Entry.unpersisted) {
          <SaveNowButton session team entry appSend />;
        } else {
          ReasonReact.null;
        }
      }
    </div>,
};