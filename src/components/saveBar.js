import React from "react";
import PropTypes from "prop-types";
import _ from "lodash";
import "./saveBar.css";
import CreateEntryService from "../services/entries/createService";

export default class SaveBar extends React.Component {
  constructor(props) {
    super(props);
    this.saveChanges = this.saveChanges.bind(this);
  }

  saveChanges() {
    console.log("Saving changes...  ");

    // Iterate over all new entries and create them on the server.
    for (const entryId in this.props.appState.unsavedEntries) {
      console.log("Creating new entry with temp ID: " + entryId);
      this.createEntry(entryId);
    }
  }

  async createEntry(unsavedEntryId) {
    const createService = new CreateEntryService(
      this.props.appState.token,
      this.props.appState.encryptionHash,
      this.props.appState.unsavedEntries[unsavedEntryId],
      this.props.appState.team.id
    );

    const savedEntryId = await createService.execute();

    console.log(
      "New entry with temp ID " +
        unsavedEntryId +
        " now has permanent ID " +
        savedEntryId +
        ". Updating..."
    );

    const entriesClone = _.cloneDeep(this.props.appState.entries);
    const unsavedEntriesClone = _.cloneDeep(this.props.appState.unsavedEntries);
    entriesClone[savedEntryId] = unsavedEntriesClone[unsavedEntryId];
    delete unsavedEntriesClone[unsavedEntryId];

    const newAppState = {
      entries: entriesClone,
      unsavedEntries: unsavedEntriesClone
    };

    if (this.props.appState.entryId === unsavedEntryId) {
      newAppState.entryId = "" + savedEntryId;
    }

    console.log("Updating appState with data: ", newAppState);

    this.props.setAppState(newAppState, () => {
      console.log("New app state should include: ", newAppState);
    });
  }

  render() {
    return (
      <div className="fixed-bottom text-center p-2 save-bar">
        <button
          className="btn btn-outline-light btn-sm"
          onClick={this.saveChanges}
        >
          Save all changes
        </button>
      </div>
    );
  }
}

SaveBar.propTypes = {
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired
};
