let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("Icon");

type kind =
  | Save
  | Submit
  | Loading
  | Success
  | Error
  | Delete;

let toText = kind =>
  switch (kind) {
  | Save => "(SAVE)"
  | Submit => "(SUBMIT)"
  | Loading => "(LOADING)"
  | Success => "(SUCCESS)"
  | Error => "(ERR)"
  | Delete => "(DEL)"
  };

let make = (~kind, _children) => {
  ...component,
  render: _self => <span className="icon"> (kind |> toText |> str) </span>,
};