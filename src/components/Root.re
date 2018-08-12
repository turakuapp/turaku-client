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
    | None => <TeamSelection ctx={userData: userData} appSend=send />
    | Some(team) => <Dashboard ctx={userData, team} appSend=send />
    }
  };

let make = _children => {
  ...app,
  initialState: () => initialState,
  reducer,
  render: ({state, send}) => <div> (currentComponent(state, send)) </div>,
};
