import React from "react";
import PropTypes from "prop-types";
import "./entries.css";
import Entry from "./entry";
import _ from "lodash";
import EntryChoice from "./entryChoice";

export default class Entries extends React.Component {
  constructor(props) {
    super(props);

    this.state = {};

    this.addEntry = this.addEntry.bind(this);
  }

  addEntry() {
    console.log("Adding a new entry...");

    const entry = {
      title: "New Entry",
      persisted: false,
      fields: [
        { name: "user", type: "user", value: "" },
        { name: "password", type: "password", value: "" },
        { name: "tags", type: "tags", value: "" }
      ]
    };

    let staleEntries = {};
    let newKey = null;

    if (_.isObject(this.props.appState.staleEntries)) {
      console.log("Stale entries exist...");
      staleEntries = _.cloneDeep(this.props.appState.staleEntries);

      const newEntryKeys = _.filter(Object.keys(staleEntries), key => {
        return _.startsWith(key, "N");
      });

      console.log("New entry keys are: ", newEntryKeys);

      if (_.isEmpty(newEntryKeys)) {
        newKey = "N1";
      } else {
        const newKeyInteger =
          parseInt(
            _.max(
              _.map(newEntryKeys, key => {
                return _.join(_.slice(key, 1));
              })
            ),
            10
          ) + 1;

        newKey = "N" + newKeyInteger;
      }
    } else {
      console.log("Stale entries do not exist...");
      newKey = "N1";
    }

    staleEntries[newKey] = entry;

    this.props.setAppState({
      entryId: newKey,
      staleEntries: staleEntries
    });
  }

  entryChoices() {
    const choices = [];

    // List all unpersisted entries.
    _.forOwn(this.props.appState.staleEntries, (staleEntry, entryId) => {
      if (staleEntry.persisted === false) {
        choices.push(
          <EntryChoice
            appState={this.props.appState}
            setAppState={this.props.setAppState}
            entryId={entryId}
            entry={staleEntry}
          />
        );
      }
    });

    // List all persisted entries.
    _.forOwn(this.props.appState.entries, (entry, entryId) => {
      choices.push(
        <EntryChoice
          appState={this.props.appState}
          setAppState={this.props.setAppState}
          entryId={entryId}
          entry={entry}
        />
      );
    });

    return choices;
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

          {this.entryChoices()}
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
