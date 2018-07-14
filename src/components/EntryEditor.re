let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  team: Team.t,
  entry: Entry.t,
};

let component = ReasonReact.statelessComponent("EntryEditor");

let handleTitleChange = (ctx, appSend, _event) => {
  Js.log("Editing title");
  let title = DomUtils.getValueOfInputById("entry-editor__title");
  appSend(Turaku.EditEntryTitle(title));
};

let fields = (ctx, appSend) => {
  let index = ref(-1);
  List.map(
    (field: Field.t) => {
      index := index^ + 1;
      <EntryField
        key=(field |> Field.key)
        ctx={
          userData: ctx.userData,
          team: ctx.team,
          entry: ctx.entry,
          field,
          index: index^,
        }
        appSend
      />;
    },
    ctx.entry |> Entry.fields,
  );
};

let make = (~ctx, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div>
      <div className="row">
        <div className="col offset-sm-2">
          <input
            _type="text"
            value=(ctx.entry |> Entry.title)
            onChange=(handleTitleChange(ctx, appSend))
            className="my-2"
            placeholder="Entry Title"
            id="entry-editor__title"
          />
        </div>
      </div>
      (fields(ctx, appSend) |> Array.of_list |> ReasonReact.array)
      <EntryTags
        ctx={team: ctx.team, entry: ctx.entry, session: ctx.userData.session}
        appSend
      />
    </div>,
};