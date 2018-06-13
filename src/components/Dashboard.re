[%bs.raw {|require("./dashboard.css")|}];

let str = ReasonReact.stringToElement;

type bag = {
  userData: Turaku.userData,
  dashboardPageData: Turaku.dashboardPageData,
};

let component = ReasonReact.statelessComponent("Dashboard");

let getMenu = (bag, appSend) =>
  switch (bag.dashboardPageData.dashboardMenu) {
  | Turaku.EntriesMenu(entryMenuData) =>
    <Entries
      bag={
        userData: bag.userData,
        dashboardPageData: bag.dashboardPageData,
        entryMenuData,
      }
      appSend
    />
  | UsersMenu => "Users menu should show up here" |> str
  };

let navigateToTeams = (bag, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  appSend(Turaku.Navigate(SignedInUser(TeamSelectionPage, bag.userData)));
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
            className="dashboard__navlink"
            onClick=(navigateToTeams(bag, appSend))>
            (str("Teams"))
          </div>
          <div className="dashboard__navlink"> (str("Users")) </div>
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