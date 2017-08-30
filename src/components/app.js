import React from "react";
import SignIn from "./signIn";
import Dashboard from "./dashboard.js";

export default class App extends React.Component {
  isSignedIn() {
    let token = this.props.appState.token;
    return typeof token !== "undefined" && token !== null;
  }

  render() {
    return (
      <div>
        {!this.isSignedIn() && (
          <SignIn
            appState={this.props.appState}
            setAppState={this.props.setAppState}
          />
        )}

        {this.isSignedIn() && (
          <Dashboard
            appState={this.props.appState}
            setAppState={this.props.setAppState}
          />
        )}
      </div>
    );
  }
}

App.propTypes = {
  appState: React.PropTypes.object,
  setAppState: React.PropTypes.func
};
