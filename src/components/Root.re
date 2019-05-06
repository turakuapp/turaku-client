[%bs.raw {|require("@fortawesome/fontawesome-free/js/all.js")|}];

open Turaku;

let app = ReasonReact.reducerComponent("App");

let str = ReasonReact.string;

let currentComponent = (state, send) =>
  switch (state) {
  | SignedOutUser(page) =>
    switch (page) {
    | LoadingPage => <LoadingScreen appSend=send />
    | SignInPage(data) => <SignInMenu data appSend=send />
    | SignUpPage => <SignUpMenu appSend=send />
    }
  | SignedInUser(userData) =>
    switch (userData.teams |> SelectableList.selected) {
    | None =>
      <TeamSelection
        session={userData.session}
        invitations={userData.invitations}
        teams={userData.teams}
        appSend=send
      />
    | Some(team) =>
      <Dashboard
        session={userData.session}
        team
        dashboardMenu={userData.dashboardMenu}
        appSend=send
      />
    }
  };

let make = _children => {
  ...app,
  initialState: () => initialState,
  reducer,
  render: ({state, send}) => <div> {currentComponent(state, send)} </div>,
};