import React from "react";
import PropTypes from "prop-types";

export default class IncomingInvitation extends React.Component {
  acceptInvitation() {
    console.log("Accepting invitation...");
  }

  rejectInvitation() {
    console.log("Rejecting invitation...");
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
          className="btn btn-sm btn-outline-dark ml-2"
        >
          Accept
        </button>

        <button
          onClick={this.rejectInvitation}
          className="btn btn-sm btn-outline-dark ml-2"
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
