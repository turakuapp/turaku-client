import React from "react";
import SignIn from "./signIn";
import SignUp from "./signUp";
import Dashboard from "./dashboard.js";
import Teams from "./teams.js";
import PropTypes from "prop-types";
import SessionRestoreService from "../services/sessions/restoreService";
import _ from "lodash";

import {
  BrowserRouter as Router,
  Redirect,
  Route,
  Switch
} from "react-router-dom";

export default class App extends React.Component {
  constructor(props) {
    super(props);

    this.state = { restorationAttempted: false };
  }

  componentWillMount() {
    // Attempt to restore a session.
    let service = new SessionRestoreService(this.props.setAppState);

    if (service.canRestore()) {
      let that = this;

      service.restore().then(() => {
        that.setState({ restorationAttempted: true });
      });
    } else {
      // Cannot restore - just proceed.
      this.setState({ restorationAttempted: true });
    }
  }

  isSignedIn() {
    return _.isString(this.props.appState.token);
  }

  render() {
    if (!this.state.restorationAttempted) {
      return <div>Restoring session...</div>;
    }

    return (
      <Router>
        <Switch>
          {!this.isSignedIn() && (
            <Redirect from="/" exact={true} to="/sign_in" />
          )}

          {this.isSignedIn() && <Redirect from="/" exact={true} to="/dash" />}

          <Route
            path="/dash"
            render={props => (
              <Dashboard
                appState={this.props.appState}
                setAppState={this.props.setAppState}
              />
            )}
          />

          <Route
            path="/sign_in"
            render={props => (
              <SignIn
                appState={this.props.appState}
                setAppState={this.props.setAppState}
              />
            )}
          />

          <Route
            path="/sign_up"
            render={props => (
              <SignUp
                appState={this.props.appState}
                setAppState={this.props.setAppState}
              />
            )}
          />

          <Route
            path="/teams"
            render={props => (
              <Teams
                appState={this.props.appState}
                setAppState={this.props.setAppState}
              />
            )}
          />
        </Switch>
      </Router>
    );
  }
}

App.propTypes = {
  appState: PropTypes.object,
  setAppState: PropTypes.func
};
