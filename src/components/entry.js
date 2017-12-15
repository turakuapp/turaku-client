import React from "react";
import PropTypes from "prop-types";
import _ from "lodash";
import Field from "./field";
import EntryTags from "./entryTags";

export default class Entry extends React.Component {
  constructor(props) {
    super(props);

    this.handleTitleChange = this.handleTitleChange.bind(this);
  }

  entrySelected() {
    return _.isString(this.props.appState.entryId);
  }

  handleTitleChange(event) {
    console.log("Changing title to " + event.target.value);

    const entryId = this.props.appState.entryId;
    const unsavedEntries = _.cloneDeep(this.props.appState.unsavedEntries);
    let unsavedEntry = unsavedEntries[entryId];

    if (!_.isObject(unsavedEntry)) {
      // The unsaved entry doesn't exist, copy it into the clone
      // of unsaved entries list from list of saved entries.
      unsavedEntry = _.cloneDeep(this.props.appState.entries[entryId]);
      unsavedEntries[entryId] = unsavedEntry;
    }

    unsavedEntry.title = event.target.value;
    this.props.setAppState({ unsavedEntries: unsavedEntries });
  }

  findEntry() {
    const entryId = this.props.appState.entryId;

    if (_.startsWith(entryId, "N")) {
      return this.props.appState.unsavedEntries[entryId];
    } else {
      const unsavedEntry = this.props.appState.unsavedEntries[entryId];
      if (_.isObject(unsavedEntry)) {
        return unsavedEntry;
      } else {
        return this.props.appState.entries[entryId];
      }
    }
  }

  entryData() {
    const entry = this.findEntry();

    return (
      <div>
        <div className="row">
          <div className="col offset-sm-2">
            <input
              type="text"
              value={entry.title}
              onChange={this.handleTitleChange}
              className="my-2"
              placeholder="Entry Title"
            />
          </div>
        </div>
        {_.map(entry.fields, field => {
          return (
            // TODO: The key passed to Field may have to be 'more unique'.
            <Field
              appState={this.props.appState}
              setAppState={this.props.setAppState}
              field={field}
              key={field.name}
            />
          );
        })}

        <EntryTags
          appState={this.props.appState}
          setAppState={this.props.setAppState}
          entry={entry}
        />
      </div>
    );
  }

  render() {
    return (
      <div>
        {!this.entrySelected() && <p>Select an entry, or create a new one.</p>}

        {this.entrySelected() && this.entryData()}
      </div>
    );
  }
}

Entry.propTypes = {
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired
};
