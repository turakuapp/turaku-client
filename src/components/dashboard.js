import React from "react";
import PropTypes from "prop-types";
import Users from "./users";
import "./dashboard.css";
import _ from "lodash";
import { Redirect, Link, Route } from "react-router-dom";
import SessionSignOutService from "../services/sessions/signOutService";
import Entries from "./entries";
import initialState from "../procedures/initialState";

export default class Dashboard extends React.Component {
  constructor(props) {
    super(props);
    this.state = { signingOut: false };
    this.signOut = this.signOut.bind(this);
    this.saveChanges = this.saveChanges.bind(this);
  }

  signOut() {
    let service = new SessionSignOutService(this.props.appState.token);
    let that = this;

    this.setState({ signingOut: true }, () => {
      service.signOut().then(() => {
        that.props.setAppState(initialState());
      });
    });
  }

  haveUnsavedEntries() {
    return !_.isEmpty(this.props.appState.unsavedEntries);
  }

  saveChanges() {
    console.log("Saving changes...");
  }

  render() {
    if (!_.isObject(this.props.appState.team)) {
      return <Redirect to="/teams" />;
    }

    return (
      <div className="container-fluid">
        <div className="row">
          <div className="col dashboard__navigation">
            <h3>Tags</h3>

            <div>
              <Link to="/dash/entries/all">All tags</Link>
            </div>

            <hr />

            <div>
              <Link to="/teams">Teams</Link>
            </div>

            <div>
              <Link to="/dash/users">Users</Link>
            </div>

            <hr />

            {!this.state.signingOut && (
              <div>
                <button
                  className="btn btn-secondary btn-sm"
                  onClick={this.signOut}
                >
                  Sign Out
                </button>
              </div>
            )}
          </div>

          <div className="col-10 dashboard__content">
            <Route
              path="/dash/entries/all"
              render={props => (
                <Entries
                  appState={this.props.appState}
                  setAppState={this.props.setAppState}
                />
              )}
            />

            <Route
              path="/dash/users"
              render={props => (
                <Users
                  appState={this.props.appState}
                  setAppState={this.props.setAppState}
                />
              )}
            />
          </div>
        </div>

        {this.haveUnsavedEntries() && (
          <div className="fixed-bottom text-center dashboard__unsaved-entries-notification p-2">
            <button
              className="btn btn-outline-light btn-sm"
              onClick={this.saveChanges}
            >
              Save all changes
            </button>
          </div>
        )}
      </div>
    );
  }
}

Dashboard.propTypes = {
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired
};
