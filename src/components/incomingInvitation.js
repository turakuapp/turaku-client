import React from "react";
import PropTypes from "prop-types";
import InvitationRejectService from "../services/invitations/rejectService";
import _ from "lodash";

export default class IncomingInvitation extends React.Component {
  constructor(props) {
    super(props);
    this.acceptInvitation = this.acceptInvitation.bind(this);
    this.rejectInvitation = this.rejectInvitation.bind(this);
  }

  acceptInvitation() {
    console.log("Accepting invitation...");
  }

  rejectInvitation() {
    console.log("Rejecting invitation...");

    let service = new InvitationRejectService(
      this.props.appState.token,
      this.props.invitation.id
    );

    service.reject().then(() => {
      // Delete own entry from appState.
      let updatedIncomingInvitations = _.cloneDeep(
        this.props.appState.incomingInvitations
      );

      _.remove(updatedIncomingInvitations, invitation => {
        console.log(
          "Comparing state invitation ID " +
            invitation.id +
            " to local invitation ID " +
            this.props.invitation.id
        );
        return invitation.id === this.props.invitation.id;
      });

      this.props.setAppState({
        incomingInvitations: updatedIncomingInvitations
      });
    });
  }

  render() {
    return (
      <li className="mb-1">
        <span>
          <strong>{this.props.invitation.team.name}</strong>, from{" "}
          <code>{this.props.invitation.inviting_user.email}</code>
        </span>

        <button
          onClick={this.acceptInvitation}
          className="btn btn-sm btn-success ml-2"
        >
          Accept
        </button>

        <button
          onClick={this.rejectInvitation}
          className="btn btn-sm btn-danger ml-2"
        >
          Reject
        </button>
      </li>
    );
  }
}

IncomingInvitation.propTypes = {
  invitation: PropTypes.object.isRequired,
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired
};
