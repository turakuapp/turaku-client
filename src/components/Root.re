type state = {restorationAttempted: bool};

type action =
  | AttemptRestoration;

let app = ReasonReact.reducerComponent("App");

let isSignedIn = () => true;

let str = ReasonReact.stringToElement;

let make = _children => {
  ...app,
  initialState: () => {restorationAttempted: false},
  reducer: (action, _state) =>
    switch action {
    | AttemptRestoration => ReasonReact.Update({restorationAttempted: true})
    },
  render: ({state}) =>
    if (! state.restorationAttempted) {
      <div> (str("Restoring session...")) </div>;
    } else {
      <div>
         (str("Session restoration complete.")) </div>;
        /* {!this.isSignedIn() && (
             <SignIn
                 appState={state}
                 appSend={send}
               />
           )}

           {this.isSignedIn() && <Dashboard
             appState={state}
                 appSend={send}
           />}

               <SignUp
               appState={state}
               appSend={send}
               />

               <Teams
               appState={state}
               appSend={send}
               /> */
    }
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