[%bs.raw {|require("./dashboard.css")|}];

let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  dashboardPageData: Turaku.dashboardPageData,
};

let component = ReasonReact.statelessComponent("Dashboard");

let getMenu = (ctx, appSend) =>
  switch (ctx.dashboardPageData.menu) {
  | Turaku.EntriesMenu(entryMenuData) =>
    <Entries
      ctx={
        userData: ctx.userData,
        dashboardPageData: ctx.dashboardPageData,
        entryMenuData,
      }
      appSend
    />
  | TeamMenu(teamMenuData) =>
    <TeamMenu
      ctx={
        userData: ctx.userData,
        dashboardPageData: ctx.dashboardPageData,
        teamMenuData,
        teamId:
          Turaku.currentTeam(ctx.userData, ctx.dashboardPageData) |> Team.id,
      }
      appSend
    />
  };

let navigateToTeams = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  appSend(
    Turaku.Navigate(
      SignedInUser({...ctx.userData, page: TeamSelectionPage}),
    ),
  );
};

let navigateToTeamMembers = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  switch (ctx.dashboardPageData.menu) {
  | TeamMenu(_) => ()
  | EntriesMenu(_) =>
    appSend(
      Turaku.Navigate(
        SignedInUser({
          ...ctx.userData,
          page:
            DashboardPage({
              ...ctx.dashboardPageData,
              menu: TeamMenu({selection: Turaku.TeamMenuLoading}),
            }),
        }),
      ),
    )
  };
};

let make = (~ctx: ctx, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div className="container-fluid">
      <div className="row">
        <div className="col dashboard__navigation">
          <Tags
            ctx={
              userData: ctx.userData,
              dashboardPageData: ctx.dashboardPageData,
            }
            appSend
          />
          <hr />
          <div
            onClick=(navigateToTeamMembers(ctx, appSend))
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
          <SignOutButton
            ctx={
              userData: ctx.userData,
              dashboardPageData: ctx.dashboardPageData,
            }
            appSend
          />
        </div>
        <div className="col-10 dashboard__content">
          (getMenu(ctx, appSend))
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