[%bs.raw {|require("./dashboard.css")|}];

let str = ReasonReact.string;

type bag = {
  userData: Turaku.userData,
  dashboardPageData: Turaku.dashboardPageData,
};

let component = ReasonReact.statelessComponent("Dashboard");

let getMenu = (bag, appSend) =>
  switch (bag.dashboardPageData.menu) {
  | Turaku.EntriesMenu(entryMenuData) =>
    <Entries
      bag={
        userData: bag.userData,
        dashboardPageData: bag.dashboardPageData,
        entryMenuData,
      }
      appSend
    />
  | TeamMenu(teamMenuData) =>
    <TeamMenu
      bag={
        userData: bag.userData,
        dashboardPageData: bag.dashboardPageData,
        teamMenuData,
        teamId:
          Turaku.currentTeam(bag.userData, bag.dashboardPageData) |> Team.id,
      }
      appSend
    />
  };

let navigateToTeams = (bag, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  appSend(
    Turaku.Navigate(
      SignedInUser({...bag.userData, page: TeamSelectionPage}),
    ),
  );
};

let navigateToTeamMembers = (bag, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  switch (bag.dashboardPageData.menu) {
  | TeamMenu(_) => ()
  | EntriesMenu(_) =>
    appSend(
      Turaku.Navigate(
        SignedInUser({
          ...bag.userData,
          page:
            DashboardPage({
              ...bag.dashboardPageData,
              menu: TeamMenu({selection: Turaku.TeamMenuLoading}),
            }),
        }),
      ),
    )
  };
};

let make = (~bag: bag, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div className="container-fluid">
      <div className="row">
        <div className="col dashboard__navigation">
          <Tags
            bag={
              userData: bag.userData,
              dashboardPageData: bag.dashboardPageData,
            }
            appSend
          />
          <hr />
          <div
            onClick=(navigateToTeamMembers(bag, appSend))
            className="dashboard__navlink">
            (str("Members"))
          </div>
          <hr />
          <div
            className="dashboard__navlink"
            onClick=(navigateToTeams(bag, appSend))>
            (str("Switch Team"))
          </div>
          <hr />
          <SignOutButton
            bag={
              userData: bag.userData,
              dashboardPageData: bag.dashboardPageData,
            }
            appSend
          />
        </div>
        <div className="col-10 dashboard__content">
          (getMenu(bag, appSend))
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