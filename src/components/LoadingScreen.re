let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("LoadingScreen");

/* TODO: This component should load necessary data, and accept password to regenerate the encryptionSalt. */
let make = (~appState, ~appSend, _children) => {
  ...component,
  didMount: _self => {
    appSend(Turaku.LoadingComplete(Session.attemptRestoration()));
    ReasonReact.NoUpdate;
  },
  render: _self => <div> ("Loading..." |> str) </div>,
};