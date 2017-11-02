import React from "react";
import PropTypes from "prop-types";
import "./entries.css";
import Entry from "./entry";
import _ from "lodash";

export default class Entries extends React.Component {
  constructor(props) {
    super(props);

    this.state = {};

    this.addEntry = this.addEntry.bind(this);
  }

  addEntry() {
    console.log("Add a new entry...");

    const entry = {
      title: "New Entry",
      fields: [
        { name: "user", type: "user", value: "" },
        { name: "password", type: "password", value: "" },
        { name: "tags", type: "tags", value: "" }
      ]
    };

    let entries = null;

    if (_.isArray(this.props.appState.entries)) {
      entries = _.cloneDeep(this.props.appState.entries).unshift(entry);
    } else {
      entries = [entry];
    }

    this.props.setAppState({
      entry: entry,
      entries: entries
    });
  }

  render() {
    return (
      <div className="row">
        <div className="col-3">
          <div className="entries__nav">
            <div className="pt-2">
              <input type="text" placeholder="Search" className="mr-2" />
              <button
                className="btn btn-primary btn-sm"
                onClick={this.addEntry}
              >
                Add new
              </button>
            </div>
          </div>
        </div>
        <div className="col-8">
          <Entry
            setAppState={this.props.setAppState}
            appState={this.props.appState}
          />
        </div>
      </div>
    );
  }
}

Entries.propTypes = {
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired
};
