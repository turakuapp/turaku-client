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
  let classes = "cursor-pointer p-2 font-thin hover:bg-white flex justify-between items-center";
  if (ctx |> isCurrentChoice) {
    classes ++ " bg-white font-normal";
  } else {
    classes;
  };
};

let chooseEntry = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  Js.log("Clicked on event choice with ID: " ++ (ctx.entry |> Entry.id));
  if (! isCurrentChoice(ctx)) {
    Turaku.SelectEntry(ctx.entry) |> appSend;
  };
};

let title = entry => {
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
      (
        if (ctx.entry |> Entry.unpersisted) {
          <SaveNowButton
            ctx={userData: ctx.userData, team: ctx.team, entry: ctx.entry}
            appSend
          />;
        } else {
          ReasonReact.null;
        }
      )
    </div>,
};