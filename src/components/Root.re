open Turaku;

let app = ReasonReact.reducerComponent("App");

let str = ReasonReact.stringToElement;

let currentComponent = (state, send) =>
  switch (state.user) {
  | SignedOutUser(page) =>
    switch (page) {
    | LoadingPage => <LoadingScreen appSend=send />
    | SignInPage(data) => <SignInMenu data appSend=send />
    | SignUpPage => <SignUpMenu appSend=send />
    }
  | SignedInUser(userData) =>
    switch (userData.page) {
    | TeamSelectionPage =>
      <TeamSelection bag={userData: userData} appSend=send />
    | DashboardPage(dashboardPageData) =>
      <Dashboard bag={userData, dashboardPageData} appSend=send />
    }
  };

let make = _children => {
  ...app,
  initialState: () => initialState,
  reducer,
  render: ({state, send}) => <div> (currentComponent(state, send)) </div>,
};