let str = ReasonReact.stringToElement;

type state = {teamPassword: string};

type action =
  | UpdateTeamPassword(string);

let component = ReasonReact.reducerComponent("Invitation");

let acceptInvitation = _event => ();

let rejectInvitation = _event => ();

let updateTeamPassword = (send, _event) => {
  let teamPassword =
    DomUtils.getValueOfInputById("invitations__team-password");
  send(UpdateTeamPassword(teamPassword));
};

let make =
    (~appState: Turaku.state, ~appSend, ~invitation: Invitation.t, _children) => {
  ...component,
  initialState: () => {teamPassword: ""},
  reducer: (action, _state) =>
    switch (action) {
    | UpdateTeamPassword(tp) => ReasonReact.Update({teamPassword: tp})
    },
  render: ({state, send}) =>
    <div className="card mb-3">
      <div className="card-body">
        <h4 className="card-title"> (str(invitation |> Invitation.name)) </h4>
        <h6 className="card-subtitle mb-2 text-muted">
          (str("from"))
          <code> (str(invitation |> Invitation.email)) </code>
        </h6>
        <p className="card-text">
          <input
            required=(Js.Boolean.to_js_boolean(true))
            id="invitations__team-password"
            className="w-full"
            _type="text"
            value=state.teamPassword
            onChange=(updateTeamPassword(send))
            placeholder="Enter your team's password to accept"
          />
        </p>
        <button
          onClick=acceptInvitation
          className="card-link btn btn-sm btn-success ml-2">
          (str("Accept"))
        </button>
        <button
          onClick=rejectInvitation
          className="card-link btn btn-sm btn-danger ml-2">
          (str("Reject"))
        </button>
      </div>
    </div>,
};
/* export default class IncomingInvitation extends React.Component {
     constructor(props) {
       super(props);

       this.state = {
         teamPassword: ""
       };

       this.acceptInvitation = this.acceptInvitation.bind(this);
       this.rejectInvitation = this.rejectInvitation.bind(this);
       this.updateTeamPassword = this.updateTeamPassword.bind(this);
     }

     acceptInvitation() {
       console.log(
         "Accepting invitation with supplied team password: " +
           this.state.teamPassword
       );

       let service = new InvitationAcceptService(
         this.props.appState.token,
         this.props.appState.encryptionHash,
         this.props.invitation.id,
         this.state.teamPassword
       );

       service.accept().then(response => {
         let updatedIncomingInvitations = this.invitationsWithoutSelf();

         // Add team entry to appState.
         let updatedTeams = _.cloneDeep(this.props.appState.teams);

         updatedTeams.push(response.team);

         this.props.setAppState({
           incomingInvitations: updatedIncomingInvitations,
           teams: updatedTeams
         });
       });
     }

     invitationsWithoutSelf() {
       let updatedIncomingInvitations = _.cloneDeep(
         this.props.appState.incomingInvitations
       );

       _.remove(updatedIncomingInvitations, invitation => {
         return invitation.id === this.props.invitation.id;
       });

       return updatedIncomingInvitations;
     }

     rejectInvitation() {
       console.log("Rejecting invitation...");

       let service = new InvitationRejectService(
         this.props.appState.token,
         this.props.invitation.id
       );

       service.reject().then(() => {
         // Delete own entry from appState.
         let updatedIncomingInvitations = this.invitationsWithoutSelf();

         this.props.setAppState({
           incomingInvitations: updatedIncomingInvitations
         });
       });
     }

     updateTeamPassword(event) {
       this.setState({ teamPassword: event.target.value });
     }

     render() {
       return (
         !!! EXTRACTED !!!
       );
     }
   }*/