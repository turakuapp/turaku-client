import React from "react";
import PropTypes from "prop-types";
import CreateService from "../services/teams/createService";
import _ from "lodash";
import { Redirect } from "react-router";

export default class Teams extends React.Component {
  haveTeams() {
    return this.props.appState.teams.length > 0;
  }

  currentTeams() {
    _.map(this.props.appState.teams, team => {
      return <li>{team.name}</li>;
    });
  }

  createTeam() {
    console.log("create a team!");
  }

  render() {
    if (!_.isArray(this.props.appState.teams)) {
      return <Redirect to="/sign_in" />;
    }

    return (
      <div>
        <h1>List of teams you're a member of:</h1>
        {this.haveTeams() && <ul>currentTeams()</ul>}
        <button onClick={this.createTeam}>Create a new Team</button>
      </div>
    );
  }
}
