import React from "react";
import PropTypes from "prop-types";

export default class AppState extends React.Component {
  constructor(props) {
    super(props);
    this.state = { page: "signIn" };
    this.setAppState = this.setAppState.bind(this);
  }

  setAppState(updater, callback) {
    // newState can be object or function!
    this.setState(updater, () => {
      if (this.props.debug) {
        console.log("setAppState", JSON.stringify(this.state));
      }

      if (callback) {
        callback();
      }
    });
  }

  render() {
    return (
      <div className="AppState">
        {React.Children.map(this.props.children, child => {
          return React.cloneElement(child, {
            appState: this.state,
            setAppState: this.setAppState
          });
        })}
      </div>
    );
  }
}

AppState.propTypes = {
  debug: PropTypes.bool
};
