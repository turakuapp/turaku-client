open Turaku;

let app = ReasonReact.reducerComponent("App");

let str = ReasonReact.stringToElement;

let currentComponent = (state, send) =>
  switch (state.currentPage) {
  | SignUpPage => <SignUpMenu appState=state appSend=send />
  | SignInPage => <SignInMenu appState=state appSend=send />
  | DashboardPage(selectedTeam, _) =>
    <Dashboard appState=state appSend=send selectedTeam />
  | TeamSelectionPage => <TeamSelection appState=state appSend=send />
  | LoadingPage => <LoadingScreen appState=state appSend=send />
  };

let make = _children => {
  ...app,
  initialState: () => initialState,
  reducer,
  render: ({state, send}) => <div> (currentComponent(state, send)) </div>,
};