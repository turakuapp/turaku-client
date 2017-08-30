import React from "react";
import PropTypes from "prop-types";
import "./dashboard.css";

export default class Dashboard extends React.Component {
  render() {
    return (
      <div className="container-fluid">
        <div className="row">
          <div className="col" styleName="dashboard__tags">
            Tags
          </div>

          <div className="col-10" styleName="dashboard__entries">
            Entries
          </div>
        </div>
      </div>
    );
  }
}

Dashboard.propTypes = {
  appState: PropTypes.object,
  setAppState: PropTypes.func
};
