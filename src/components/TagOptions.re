[%bs.raw {|require("./TagOptions.css")|}];

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("TagOptions");

let make = (~team, ~entry, ~search, _children) => {
  ...component,
  render: _self =>
    if (search |> String.length < 2) {
      ReasonReact.null;
    } else {
      let matchingTags =
        team
        |> Team.tags
        |> SelectableList.all
        |> List.filter(tag => {
             let searchExp = search |> Js.Re.fromStringWithFlags(~flags="i");
             tag |> Tag.name |> Js.String.search(searchExp) > (-1) ?
               entry
               |> Entry.tagIds
               |> List.for_all(tagId => tagId != (tag |> Tag.id)) :
               false;
           });

      switch (matchingTags) {
      | [] => ReasonReact.null
      | tags =>
        <div className="tag-options__container">
          (
            tags
            |> Array.of_list
            |> Array.map(tag =>
                 <div className="tag-options__option" key=(tag |> Tag.id)>
                   (tag |> Tag.name |> str)
                 </div>
               )
            |> ReasonReact.array
          )
        </div>
      };
    },
};