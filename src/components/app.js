import React from "react";
import SignIn from "./signIn";
import SignUp from "./signUp";
import Dashboard from "./dashboard.js";
import PropTypes from "prop-types";

import {
  BrowserRouter as Router,
  Redirect,
  Route,
  Switch
} from "react-router-dom";

export default class App extends React.Component {
  isSignedIn() {
    let token = this.props.appState.token;
    return typeof token !== "undefined" && token !== null;
  }

  render() {
    return (
      <Router>
        <Switch>
          {!this.isSignedIn() && (
            <Redirect from="/" exact={true} to="/sign_in" />
          )}

          {this.isSignedIn() && (
            <Route
              path="/"
              exact={true}
              render={props => (
                <Dashboard
                  appState={this.props.appState}
                  setAppState={this.props.setAppState}
                />
              )}
            />
          )}

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
        </Switch>
      </Router>
    );
  }
}

App.propTypes = {
  appState: PropTypes.object,
  setAppState: PropTypes.func
};
