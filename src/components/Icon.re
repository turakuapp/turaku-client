let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("Icon");

type kind =
  | Submit
  | Loading
  | Success
  | Error;

let toText = kind =>
  switch (kind) {
  | Submit => "(SUBMIT)"
  | Loading => "(LOADING)"
  | Success => "(SUCCESS)"
  | Error => "(ERROR)"
  };

let make = (~kind, _children) => {
  ...component,
  render: _self => <span className="icon"> (kind |> toText |> str) </span>,
};