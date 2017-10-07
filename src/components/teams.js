import React from "react";
import PropTypes from "prop-types";
import CreateService from "../services/teams/createService";
import _ from "lodash";
import { Redirect } from "react-router";
import "./teams.css";

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
  }

  haveTeams() {
    return this.props.appState.teams.length > 0;
  }

  selectTeam(team) {
    this.props.setAppState({ team: team }, () => {
      this.setState({ teamSelected: true }, () => {
        // Store the team in session storage as well, so that when reloading,
        // it can be automatically set as selected team.
        sessionStorage.setItem("team", JSON.stringify(team));
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
    let createService = new CreateService(this.props.appState.token);
    let teamName = document.getElementById("teams__form-name").value;
    let that = this;

    createService
      .create(teamName)
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
    this.setState({ createFormVisible: true });
  }

  hideCreateForm() {
    this.setState({ createFormVisible: false });
  }

  render() {
    if (!_.isArray(this.props.appState.teams)) {
      return <Redirect to="/sign_in" />;
    }

    if (this.state.teamSelected) {
      return <Redirect to="/" />;
    }

    return (
      <div className="container">
        <div className="row justify-content-center sign-in__centered-container">
          <div className="col-md-6 align-self-center">
            {this.haveTeams() && (
              <div>
                <h2>Your Teams</h2>
                <ul className="mt-3 teams__ul">{this.currentTeams()}</ul>
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
