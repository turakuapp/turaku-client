let str = ReasonReact.string;

module FontAwesomeIcon = {
  [@bs.module "@fortawesome/react-fontawesome"]
  external fontAwesomeIcon : ReasonReact.reactClass = "FontAwesomeIcon";

  type fontAwesomeSolidSvgIcon;

  [@bs.module "@fortawesome/free-solid-svg-icons"]
  external faCoffee : fontAwesomeSolidSvgIcon = "";

  [@bs.module "@fortawesome/free-solid-svg-icons"]
  external faSave : fontAwesomeSolidSvgIcon = "";

  [@bs.module "@fortawesome/free-solid-svg-icons"]
  external faTrash : fontAwesomeSolidSvgIcon = "";

  [@bs.module "@fortawesome/free-solid-svg-icons"]
  external faExclamationCircle : fontAwesomeSolidSvgIcon = "";

  [@bs.module "@fortawesome/free-solid-svg-icons"]
  external faCheckCircle : fontAwesomeSolidSvgIcon = "";

  [@bs.module "@fortawesome/free-solid-svg-icons"]
  external faSpinner : fontAwesomeSolidSvgIcon = "";

  [@bs.module "@fortawesome/free-solid-svg-icons"]
  external faCloudUpload : fontAwesomeSolidSvgIcon = "";

  [@bs.deriving abstract]
  type jsProps = {
    icon: fontAwesomeSolidSvgIcon,
    size: Js.nullable(string),
    fixedWidth: Js.nullable(bool),
    inverse: Js.nullable(bool),
    spin: Js.nullable(bool),
    pulse: Js.nullable(bool),
  };

  let make =
      (~icon, ~size=?, ~fixedWidth=?, ~inverse=?, ~spin=?, ~pulse=?, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=fontAwesomeIcon,
      ~props=
        jsProps(
          ~icon,
          ~size=Js.Nullable.fromOption(size),
          ~fixedWidth=Js.Nullable.fromOption(fixedWidth),
          ~inverse=Js.Nullable.fromOption(inverse),
          ~spin=Js.Nullable.fromOption(spin),
          ~pulse=Js.Nullable.fromOption(pulse),
        ),
      children,
    );
};

let component = ReasonReact.statelessComponent("Icon");

module Size = {
  type t =
    | Xs
    | Sm
    | Md
    | Lg
    | X2
    | X3
    | X5
    | X7
    | X10;

  let toString = size =>
    switch (size) {
    | Xs => Some("xs")
    | Sm => Some("sm")
    | Md => None
    | Lg => Some("lg")
    | X2 => Some("2x")
    | X3 => Some("3x")
    | X5 => Some("5x")
    | X7 => Some("7x")
    | X10 => Some("10x")
    };
};

type kind =
  | Save
  | Submit
  | Loading
  | Success
  | Error
  | Delete;

let reactIcon = (kind, size) => {
  let size = Size.toString(size);

  let icon =
    switch (kind) {
    | Save => FontAwesomeIcon.faSave
    | Submit => FontAwesomeIcon.faCloudUpload
    | Loading => FontAwesomeIcon.faSpinner
    | Success => FontAwesomeIcon.faCheckCircle
    | Error => FontAwesomeIcon.faExclamationCircle
    | Delete => FontAwesomeIcon.faTrash
    };

  let pulse =
    switch (kind) {
    | Save
    | Submit
    | Success
    | Error
    | Delete => false
    | Loading => true
    };

  <FontAwesomeIcon icon ?size pulse />;
};

let make = (~kind, ~size, _children) => {
  ...component,
  render: _self => <span className="icon"> (reactIcon(kind, size)) </span>,
};