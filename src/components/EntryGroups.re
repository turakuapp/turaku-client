let str = React.string;

[@react.component]
let make = _children =>
  <div className="mt-4">
    <div className="uppercase text-xs font-bold tracking-wide pl-4 mb-1">
      {"Groups" |> str}
    </div>
    <div
      className="cursor-pointer p-2 pl-4 font-thin hover:bg-grey-lighter text-sm">
      {"Group A" |> str}
    </div>
    <div
      className="cursor-pointer p-2 pl-4 font-thin hover:bg-grey-lighter text-sm">
      {"Group B" |> str}
    </div>
  </div>;