[%bs.raw {|require("./dashboard.css")|}];

type state = {signingOut: bool};

let str = ReasonReact.stringToElement;

type action =
  | SignOut;

let component = ReasonReact.reducerComponent("Dashboard");

let getMenu = (appState, appSend, selectedTeam, dashboardMenu) =>
  switch (dashboardMenu) {
  | Turaku.EntriesMenu(entrySelection) =>
    <Entries appState appSend selectedTeam entrySelection />
  | UsersMenu => "Users menu should show up here" |> str
  };

let make = (~appState, ~appSend, ~selectedTeam, ~dashboardMenu, _children) => {
  ...component,
  initialState: () => {signingOut: false},
  reducer: (action, _state) =>
    switch (action) {
    | SignOut =>
      ReasonReact.UpdateWithSideEffects(
        {signingOut: true},
        (_self => appSend(Turaku.SignOut)),
      )
    },
  render: ({state, send}) =>
    <div className="container-fluid">
      <div className="row">
        <div className="col dashboard__navigation">
          <Tags appState appSend />
          <hr />
          <div> <a href="#"> (str("Teams")) </a> </div>
          <div> <a href="#"> (str("Users")) </a> </div>
          <hr />
          <SignOutButton appSend appState />
        </div>
        <div className="col-10 dashboard__content">
          (dashboardMenu |> getMenu(appState, appSend, selectedTeam))
          /* <Users appState appSend /> */
          <span />
        </div>
      </div>
    </div>,
  /* {this.haveUnsavedEntries() && ( */
  /* <SaveBar appState=appState appSend=appSend /> */
  /* )} */
};
/* export default class Dashboard extends React.Component {
     constructor(props) {
       super(props);
       this.state = { signingOut: false };
       this.signOut = this.signOut.bind(this);
     }

     signOut() {
       let service = new SessionSignOutService(this.props.appState.token);
       let that = this;

       this.setState({ signingOut: true }, () => {
         service.signOut().then(() => {
           that.props.setAppState(initialState());
         });
       });
     }

     haveUnsavedEntries() {
       return !_.isEmpty(this.props.appState.unsavedEntries);
     }

     render() {
       if (!_.isObject(this.props.appState.team)) {
         return <Redirect to="/teams" />;
       }

       return (
         !!!!!!!!!
         EXTRACTED
         !!!!!!!!!
       );
     }
   } */