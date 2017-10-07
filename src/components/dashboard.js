import React from "react";
import PropTypes from "prop-types";
import Users from "./users";
import "./dashboard.css";
import _ from "lodash";
import { Redirect } from "react-router-dom";

export default class Dashboard extends React.Component {
  render() {
    if (!_.isObject(this.props.appState.team)) {
      return <Redirect to="/teams" />;
    }

    return (
      <div className="container-fluid">
        <div className="row">
          <div className="col dashboard__tags">Tags</div>

          <div className="col-10 dashboard__content">
            <Users
              appState={this.props.appState}
              setAppState={this.props.setAppState}
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
