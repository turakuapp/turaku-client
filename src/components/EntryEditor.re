let str = React.string;

let handleTitleChange = (appSend, _event) => {
  Js.log("Editing title");
  let title = DomUtils.getValueOfInputById("entry-editor__title");
  appSend(Turaku.EditEntryTitle(title));
};

let fields = (entry, appSend) => {
  let index = ref(-1);
  List.map(
    (field: Field.t) => {
      index := index^ + 1;
      <EntryField key={field |> Field.key} entry field index=index^ appSend />;
    },
    entry |> Entry.fields,
  );
};

[@react.component]
let make = (~team, ~entry, ~session, ~appSend, _children) =>
  <div className="px-2">
    <div className="flex mt-2">
      <div className="w-32 mr-2" />
      <input
        type_="text"
        value={entry |> Entry.title}
        onChange={handleTitleChange(appSend)}
        className="w-1/2 text-lg p-2 bg-white hover:bg-grey-lighter focus:bg-grey-lighter font-bold"
        placeholder="Entry Title"
        id="entry-editor__title"
      />
    </div>
    {fields(entry, appSend) |> Array.of_list |> ReasonReact.array}
    <EntryTags team entry session appSend />
  </div>;