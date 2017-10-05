import React from "react";
import PropTypes from "prop-types";
import _ from "lodash";
import ListUsersService from "../services/teams/listUsersService";

export default class Users extends React.Component {
  componentDidMount() {
    let service = new ListUsersService(
      this.props.appState.team,
      this.props.appState.token
    );

    service
      .list()
      .then(users => {
        // TODO: Sync users instead of over-writing if already loaded.
        this.props.setAppState({ users: users });
      })
      .catch(exception => {
        // TODO: Handle load error here.
        console.log(exception, "List users failed. What do we do? :-(");
      });
  }

  listOfUsers() {
    if (_.isArray(this.props.appState.users)) {
      return (
        <ul>
          {_.map(this.props.appState.users, user => {
            return <li key={user.id}>{user.name}</li>;
          })}
        </ul>
      );
    } else {
      return <span>Loading users...</span>;
    }
  }

  render() {
    return (
      <div>
        <h2>Users</h2>
        {this.listOfUsers()}
      </div>
    );
  }
}

Users.propTypes = {
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired
};
