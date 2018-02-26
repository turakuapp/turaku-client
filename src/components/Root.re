type page =
  | SignUp;

type state = {
  restorationAttempted: bool,
  currentPage: page
};

type action =
  | AttemptRestoration;

let app = ReasonReact.reducerComponent("App");

let isSignedIn = () => true;

let str = ReasonReact.stringToElement;

let currentComponent = (state, send) =>
  switch state.currentPage {
  | SignUp => <SignUp appState=state appSend=send />
  };

let make = _children => {
  ...app,
  initialState: () => {restorationAttempted: true, currentPage: SignUp},
  reducer: (action, state) =>
    switch action {
    | AttemptRestoration =>
      ReasonReact.Update({...state, restorationAttempted: true})
    },
  render: ({state, send}) =>
    if (! state.restorationAttempted) {
      <div> (str("Restoring session...")) </div>;
    } else {
      <div>
        (str("Session restoration complete."))
        /* {!this.isSignedIn() && (
             <SignIn
                 appState={state}
                 appSend={send}
               />
           )}

           {this.isSignedIn() && <Dashboard
             appState={state}
                 appSend={send}
           />} */
        (currentComponent(state, send))
      </div>;
    }
  /* <Teams
     appState={state}
     appSend={send}
     /> */
};
/* componentWillMount() {
       // Attempt to restore a session.
       let service = new SessionRestoreService(this.props.setAppState);

       if (service.canRestore()) {
         let that = this;

         service.restore().then(() => {
           that.setState({ restorationAttempted: true });
         });
       } else {
         // Cannot restore - just proceed.
         this.setState({ restorationAttempted: true });
       }
     }
   } */