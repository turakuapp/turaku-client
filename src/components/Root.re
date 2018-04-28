open Turaku;

let app = ReasonReact.reducerComponent("App");

let str = ReasonReact.stringToElement;

let currentComponent = (state, send) =>
  switch (state.currentPage) {
  | SignUpPage => <SignUp appState=state appSend=send />
  | SignInPage => <SignIn appState=state appSend=send />
  | DashboardPage(_) => <Dashboard appState=state appSend=send />
  };

let make = _children => {
  ...app,
  initialState: () => initialState,
  reducer: (action, state) =>
    switch (action) {
    | SignedIn => ReasonReact.NoUpdate
    | SignedUp => ReasonReact.Update({...state, currentPage: SignInPage})
    },
  render: ({state, send}) => {
    if (!state.restorationAttempted) {
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
    }
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