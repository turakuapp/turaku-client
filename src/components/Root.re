open Turaku;

let app = ReasonReact.reducerComponent("App");

let str = ReasonReact.stringToElement;

let currentComponent = (state, send) =>
  switch (state.user) {
  | SignedOut(page) =>
    switch (page) {
    | LoadingPage => <LoadingScreen appSend=send />
    | SignInPage(data) => <SignInMenu data appSend=send />
    | SignUpPage => <SignUpMenu appSend=send />
    }
  | SignedIn(page, signedInData) =>
    switch (page) {
    | TeamSelectionPage => <TeamSelection signedInData appSend=send />
    | DashboardPage(dashboardData) =>
      <Dashboard signedInData=data dashboardData />
    }
  };

let make = _children => {
  ...app,
  initialState: () => initialState,
  reducer,
  render: ({state, send}) => <div> (currentComponent(state, send)) </div>,
};