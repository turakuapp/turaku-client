[%bs.raw {|require("./EntryTag.css")|}];

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("EntryTag");

let make = (~team, ~tagId, _children) => {
  ...component,
  render: self => {
    let tag =
      team
      |> Team.tags
      |> SelectableList.all
      |> List.find(tag => tag |> Tag.id == tagId);

    <span className="p-1 mr-1 entry-tag__container">
      (tag |> Tag.name |> str)
      (" " |> str)
      <Icon kind=Icon.Delete />
    </span>;
  },
};