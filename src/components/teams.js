import React from "react";
import PropTypes from "prop-types";
import CreateService from "../services/teams/createService";
import _ from "lodash";
import { Redirect } from "react-router";
import "./teams.css";
import IncomingInvitation from "./incomingInvitation";
import TeamPasswordService from "../services/teams/passwordService";
import CryptoService from "../services/cryptoService";

export default class Teams extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      createFormVisible: false,
      teamSelected: false
    };

    this.createTeam = this.createTeam.bind(this);
    this.showCreateForm = this.showCreateForm.bind(this);
    this.hideCreateForm = this.hideCreateForm.bind(this);
    this.selectTeam = this.selectTeam.bind(this);
    this.updateTeamPassword = this.updateTeamPassword.bind(this);
  }

  haveTeams() {
    return this.props.appState.teams.length > 0;
  }

  haveIncomingInvitations() {
    return this.props.appState.incomingInvitations.length > 0;
  }

  async selectTeam(team) {
    // Decrypt the password and store that now that a team has been selected.
    let password = await new CryptoService(
      this.props.appState.encryptionHash
    ).decrypt(team.encrypted_password);

    let decryptedTeam = _.clone(team);
    delete decryptedTeam.encrypted_password;
    decryptedTeam.password = password;

    this.props.setAppState({ team: decryptedTeam }, () => {
      this.setState({ teamSelected: true }, () => {
        // Store the team in session storage as well, so that when reloading,
        // it can be automatically set as selected team.
        sessionStorage.setItem("team", JSON.stringify(decryptedTeam));
      });
    });
  }

  currentTeams() {
    return _.map(this.props.appState.teams, team => {
      return (
        <li className="mb-1" key={team.id}>
          <button
            onClick={_.partial(this.selectTeam, team)}
            className="btn btn-sm btn-outline-dark"
          >
            {team.name}
          </button>
        </li>
      );
    });
  }

  createTeam(event) {
    event.preventDefault();

    let createService = new CreateService(
      this.props.appState.token,
      this.props.appState.encryptionHash
    );

    let name = document.getElementById("teams__form-name").value;
    let that = this;

    createService
      .create(name, this.state.teamPassword)
      .then(team => {
        console.log("Created team.");

        let updatedTeams = _.cloneDeep(that.props.appState.teams);
        updatedTeams.push(team);

        that.props.setAppState({ teams: updatedTeams }, () => {
          that.setState({ createFormVisible: false });
        });
      })
      .catch(exception => {
        // Handle invalid credentials / exception.
        console.log(exception, "Team creation failed.");
      });
  }

  async updateTeamPassword(event) {
    if (_.isObject(event)) {
      console.log(
        "Hashing " + event.target.value + " to get new team password..."
      );

      const updatedPassword = await new TeamPasswordService().updatePassword(
        event.target.value
      );

      console.log("Updated team password is " + updatedPassword);

      this.setState({ teamPassword: updatedPassword });
    }
  }

  createForm() {
    return (
      <form onSubmit={this.createTeam}>
        <div className="form-group">
          <label htmlFor="teams__form-name">Name of your team</label>
          <input
            className="form-control"
            id="teams__form-name"
            aria-describedby="teams__form-name-help"
            placeholder="Enter your team's name"
          />
          <small id="teams__form-name-help" className="form-text text-muted">
            You can add team members later.
          </small>
        </div>

        <div className="form-group">
          <label htmlFor="teams__form-password">Team Password</label>
          <input
            className="form-control"
            id="teams__form-password"
            aria-describedby="teams__form-password-help"
            value={this.state.teamPassword}
            onChange={this.updateTeamPassword}
          />
          <small
            id="teams__form-password-help"
            className="form-text text-muted"
          >
            <ul>
              <li>
                Type random characters into the password field to improve its{" "}
                <em>randomness</em>.
              </li>
              <li>This is the password used to encode your team's records.</li>
              <li>It will be encrypted before being sent to Turaku.</li>
            </ul>
          </small>
        </div>

        <button
          type="submit"
          className="btn btn-primary"
          onClick={this.createTeam}
        >
          Create
        </button>

        <button
          className="btn btn-secondary ml-2"
          onClick={this.hideCreateForm}
        >
          Cancel
        </button>
      </form>
    );
  }

  showCreateForm() {
    this.setState({
      createFormVisible: true,
      teamPassword: new TeamPasswordService().newPassword()
    });
  }

  hideCreateForm() {
    this.setState({ createFormVisible: false });
  }

  incomingInvitations() {
    return _.map(this.props.appState.incomingInvitations, invitation => {
      return (
        <IncomingInvitation
          appState={this.props.appState}
          setAppState={this.props.setAppState}
          invitation={invitation}
          key={invitation.team.id}
        />
      );
    });
  }

  render() {
    if (!_.isArray(this.props.appState.teams)) {
      return <Redirect to="/sign_in" />;
    }

    if (this.state.teamSelected) {
      return <Redirect to="/dash" />;
    }

    return (
      <div className="container">
        <div className="row justify-content-center sign-in__centered-container">
          <div className="col-md-6 align-self-center">
            {this.haveTeams() && (
              <div>
                <h2>Your Teams</h2>
                <ul className="mt-3 teams__ul">{this.currentTeams()}</ul>
                <hr />
              </div>
            )}

            {this.haveIncomingInvitations() && (
              <div>
                <h2>Invitations</h2>
                <p>You have been invited to join:</p>
                <ul className="mt-3 teams_ul">{this.incomingInvitations()}</ul>
              </div>
            )}

            {!this.haveTeams() &&
              !this.state.createFormVisible && (
                <p>You do not belong to any team, right now. Create one?</p>
              )}

            {!this.state.createFormVisible && (
              <button className="btn btn-primary" onClick={this.showCreateForm}>
                Create a new Team
              </button>
            )}

            {this.state.createFormVisible && this.createForm()}
          </div>
        </div>
      </div>
    );
  }
}

Teams.propTypes = {
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired
};
