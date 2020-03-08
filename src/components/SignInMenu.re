let str = React.string;

[@react.component]
let make = (~log, ~signIn) =>
  <div> {"The sign in menu should go here." |> str} </div>;
