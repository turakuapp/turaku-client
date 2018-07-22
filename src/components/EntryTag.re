let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("EntryTag");

let removeTag = (entry, tagId, appSend, event) =>
  appSend(Turaku.RemoveTag(entry, tagId));

let make = (~team, ~entry, ~tagId, ~appSend, _children) => {
  ...component,
  render: self => {
    /* This uses the unsafe List.find. Use find_option / find_opt when it becomes available. */
    let tag =
      team
      |> Team.tags
      |> SelectableList.all
      |> List.find(tag => tag |> Tag.id == tagId);

    <span className="p-1 mr-1" onClick=(removeTag(entry, tagId, appSend))>
      (tag |> Tag.name |> str)
      (" " |> str)
      <Icon kind=Icon.Delete />
    </span>;
  },
};