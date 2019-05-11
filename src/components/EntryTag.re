let str = React.string;

let removeTag = (entry, tagId, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  appSend(Turaku.RemoveTag(entry, tagId));
};

[@react.component]
let make = (~team, ~entry, ~tagId, ~appSend) => {
  /* This uses the unsafe List.find. Use find_option / find_opt when it becomes available. */
  let tag =
    team
    |> Team.tags
    |> SelectableList.all
    |> List.find(tag => tag |> Tag.id == tagId);

  <span className="p-1 mr-1" onClick={removeTag(entry, tagId, appSend)}>
    {tag |> Tag.name |> str}
    {" " |> str}
    <i className="fas fa-trash fa-xs" />
  </span>;
};