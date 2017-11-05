import React from "react";
import PropTypes from "prop-types";
import _ from "lodash";
import Field from "./field";

export default class Entry extends React.Component {
  constructor(props) {
    super(props);

    this.handleTitleChange = this.handleTitleChange.bind(this);
  }

  entrySelected() {
    return _.isString(this.props.appState.entryId);
  }

  handleTitleChange(event) {
    console.log("Title has changed to " + event.target.value);
  }

  findEntry() {
    const entryId = this.props.appState.entryId;

    if (_.startsWith(entryId, "N")) {
      return this.props.appState.staleEntries[entryId];
    } else {
      const staleEntry = this.props.appState.staleEntries[entryId];
      if (_.isObject(staleEntry)) {
        return staleEntry;
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
