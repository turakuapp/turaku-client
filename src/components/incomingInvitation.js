import React from "react";
import PropTypes from "prop-types";
import InvitationRejectService from "../services/invitations/rejectService";
import InvitationAcceptService from "../services/invitations/acceptService";
import _ from "lodash";

export default class IncomingInvitation extends React.Component {
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
      <div className="card mb-3">
        <div className="card-body">
          <h4 className="card-title">{this.props.invitation.team.name}</h4>
          <h6 className="card-subtitle mb-2 text-muted">
            from <code>{this.props.invitation.inviting_user.email}</code>
          </h6>
          <p className="card-text">
            <input
              type="text"
              value={this.state.teamPassword}
              onChange={this.updateTeamPassword}
              placeholder="Enter your team's password to accept"
              style={{ width: "100%" }}
            />
          </p>

          <button
            onClick={this.acceptInvitation}
            className="card-link btn btn-sm btn-success ml-2"
          >
            Accept
          </button>

          <button
            onClick={this.rejectInvitation}
            className="card-link btn btn-sm btn-danger ml-2"
          >
            Reject
          </button>
        </div>
      </div>
    );
  }
}

IncomingInvitation.propTypes = {
  invitation: PropTypes.object.isRequired,
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired
};
