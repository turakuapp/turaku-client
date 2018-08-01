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
    size: string,
    fixedWidth: Js.nullable(bool),
    inverse: Js.nullable(bool),
    spin: Js.nullable(bool),
    pulse: Js.nullable(bool),
  };

  let make =
      (~icon, ~size, ~fixedWidth=?, ~inverse=?, ~spin=?, ~pulse=?, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=fontAwesomeIcon,
      ~props=
        jsProps(
          ~icon,
          ~size,
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
    | Lg
    | X2
    | X3
    | X5
    | X7
    | X10;

  let toString = size =>
    switch (size) {
    | Xs => "xs"
    | Sm => "sm"
    | Lg => "lg"
    | X2 => "2x"
    | X3 => "3x"
    | X5 => "5x"
    | X7 => "7x"
    | X10 => "10x"
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
  switch (kind) {
  | Save => <FontAwesomeIcon icon=FontAwesomeIcon.faSave size />
  | Submit => <FontAwesomeIcon icon=FontAwesomeIcon.faCloudUpload size />
  | Loading =>
    <FontAwesomeIcon icon=FontAwesomeIcon.faSpinner size pulse=true />
  | Success => <FontAwesomeIcon icon=FontAwesomeIcon.faCheckCircle size />
  | Error => <FontAwesomeIcon icon=FontAwesomeIcon.faExclamationCircle size />
  | Delete => <FontAwesomeIcon icon=FontAwesomeIcon.faTrash size />
  };
};

let make = (~kind, ~size, _children) => {
  ...component,
  render: _self => <span className="icon"> (reactIcon(kind, size)) </span>,
};