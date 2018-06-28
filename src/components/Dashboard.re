[%bs.raw {|require("./dashboard.css")|}];

let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  team: Team.t,
};

let component = ReasonReact.statelessComponent("Dashboard");

let getMenu = (ctx, appSend) =>
  switch (ctx.userData.dashboardMenu) {
  | Turaku.EntriesMenu =>
    <Entries ctx={userData: ctx.userData, team: ctx.team} appSend />
  | TeamMenu(teamMenuSelection) =>
    <TeamMenu
      ctx={userData: ctx.userData, team: ctx.team, teamMenuSelection}
      appSend
    />
  };

let navigateToTeams = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  Turaku.DeselectTeam(ctx.userData) |> appSend;
};

let navigateToTeamMenu = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  switch (ctx.userData.dashboardMenu) {
  | TeamMenu(_) => ()
  | EntriesMenu => Turaku.SelectTeamMenu(ctx.userData) |> appSend
  };
};

let make = (~ctx: ctx, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div className="container-fluid">
      <div className="row">
        <div className="col dashboard__navigation">
          <Tags ctx={userData: ctx.userData} appSend />
          <hr />
          <div
            onClick=(navigateToTeamMenu(ctx, appSend))
            className="dashboard__navlink">
            (str("Members"))
          </div>
          <hr />
          <div
            className="dashboard__navlink"
            onClick=(navigateToTeams(ctx, appSend))>
            (str("Switch Team"))
          </div>
          <hr />
          <SignOutButton ctx={userData: ctx.userData} appSend />
        </div>
        <div className="col-10 dashboard__content">
          (getMenu(ctx, appSend))
          <span />
        </div>
      </div>
    </div>,
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