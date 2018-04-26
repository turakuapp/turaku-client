type page =
  | SignUp
  | SignIn
  | Dashboard;

type state = {
  restorationAttempted: bool,
  currentPage: page,
};

let app = ReasonReact.reducerComponent("App");

let isSignedIn = () => true;

let str = ReasonReact.stringToElement;

let currentComponent = (state, send) =>
  switch (state.currentPage) {
  | SignUp => <SignUp appState=state appSend=send />
  | SignIn => <SignIn appState=state appSend=send />
  | Dashboard => <Dashboard appState=state appSend=send />
  };

let make = _children => {
  ...app,
  initialState: () => {restorationAttempted: true, currentPage: SignUp},
  reducer: (action: Turaku.action, state) =>
    switch (action) {
    | AttemptRestoration =>
      ReasonReact.Update({...state, restorationAttempted: true})
    | SignedIn => ReasonReact.NoUpdate
    | SignedUp => ReasonReact.Update({...state, currentPage: SignIn})
    },
  render: ({state, send}) =>
    if (! state.restorationAttempted) {
      <div> (str("Restoring session...")) </div>;
    } else {
      <div>
         (currentComponent(state, send)) </div>;
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
    },
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