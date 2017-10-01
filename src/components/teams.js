import React from "react";
import PropTypes from "prop-types";
import CreateService from "../services/teams/createService";
import _ from "lodash";
import { Redirect } from "react-router";

export default class Teams extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      createFormVisible: false
    };

    this.createTeam = this.createTeam.bind(this);
    this.showCreateForm = this.showCreateForm.bind(this);
    this.hideCreateForm = this.hideCreateForm.bind(this);
  }

  haveTeams() {
    return this.props.appState.teams.length > 0;
  }

  currentTeams() {
    return _.map(this.props.appState.teams, team => {
      return <li key={team.id}>{team.name}</li>;
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

    return (
      <div className="container">
        <div className="row justify-content-center sign-in__centered-container">
          <div className="col-md-6 align-self-center">
            {this.haveTeams() && (
              <div>
                <h2>Your Teams</h2>
                <ul>{this.currentTeams()}</ul>
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
  appState: PropTypes.object,
  setAppState: PropTypes.func
};
