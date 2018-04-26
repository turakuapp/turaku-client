/* import "./dashboard.css"; */
type state = {signingOut: bool};

let str = ReasonReact.stringToElement;

type action =
  | SignOut;

let component = ReasonReact.reducerComponent("Dashboard");

let signOutButton = (signingOut) => {
if (signingOut) {
<button className="btn btn-secondary btn-sm">Signing out...</button>
} else {
                   <button
                     className="btn btn-secondary btn-sm"
                     onClick={this.signOut}
                   >
                     Sign Out
                   </button>
}
};

let make = (~appState, ~appSend, _children) => {
  ...component,
  initialState: () => {signingOut: false},
  reducer: (action, state) => {
    switch(action) {
    | SignOut => ReasonReact.update(...state, signingOut: true)
    }
  },
  render: ({state, send}) => {
    <div className="container-fluid">
      <div className="row">
        <div className="col dashboard__navigation">
          <Tags appState=appState appSend=appSend />
          <hr />
          <div>
            <a href="#">Teams</a>
          </div>

          <div>
          <a href="#">Users</a>
          </div>

          <hr />

          signOutButton(state.signingOut)
        </div>

        <div className="col-10 dashboard__content">
          <Entries appState=appState appSend=appSend />
          <Users appState=appState appSend=appSend />
        </div>
      </div>

      /* {this.haveUnsavedEntries() && ( */
        /* <SaveBar appState=appState appSend=appSend /> */
      /* )} */
    </div>
  }
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