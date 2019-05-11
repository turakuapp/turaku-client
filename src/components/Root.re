[%bs.raw {|require("@fortawesome/fontawesome-free/js/all.js")|}];

open Turaku;

let str = React.string;

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

[@react.component]
let make = () => {
  let (state, send) = React.useReducer(Turaku.reducer, Turaku.initialState);

  <div> {currentComponent(state, send)} </div>;
};