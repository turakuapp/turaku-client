import React from "react";
import PropTypes from "prop-types";
import Users from "./users";
import "./dashboard.css";
import _ from "lodash";
import { Redirect, Link, Route } from "react-router-dom";
import SessionSignOutService from "../services/sessions/signOutService";

export default class Dashboard extends React.Component {
  constructor(props) {
    super(props);
    this.state = { signingOut: false };
    this.signOut = this.signOut.bind(this);
  }

  signOut() {
    let service = new SessionSignOutService(this.props.appState.token);
    let that = this;

    this.setState({ signingOut: true }, () => {
      service.signOut().then(() => {
        that.props.setAppState({
          token: undefined,
          team: undefined,
          teams: undefined,
          encryptionHash: undefined,
          incomingInvitations: undefined
        });
      });
    });
  }

  render() {
    if (!_.isObject(this.props.appState.team)) {
      return <Redirect to="/teams" />;
    }

    return (
      <div className="container-fluid">
        <div className="row">
          <div className="col dashboard__navigation">
            <div>Tags</div>
            <div>
              <Link to="/teams">Teams</Link>
            </div>
            <div>
              <Link to="/dash/users">Users</Link>
            </div>
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
      </div>
    );
  }
}

Dashboard.propTypes = {
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired
};
