let str = React.string;

let tagLinks = (team, _appSend) =>
  team
  |> Team.tags
  |> SelectableList.all
  |> List.map(tag =>
       <div
         className="cursor-pointer p-2 text-sm pl-4 font-thin hover:bg-grey-lighter"
         key={tag |> Tag.id}>
         {tag |> Tag.name |> str}
       </div>
     )
  |> Array.of_list
  |> React.array;

[@react.component]
let make = (~team, ~appSend) =>
  <div className="mt-4">
    <div className="uppercase text-xs font-bold tracking-wide pl-4 mb-1">
      {"Tags" |> str}
    </div>
    {tagLinks(team, appSend)}
  </div>;