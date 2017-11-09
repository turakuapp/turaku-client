import React from "react";
import PropTypes from "prop-types";
import _ from "lodash";

export default class Field extends React.Component {
  constructor(props) {
    super(props);

    this.handleChange = this.handleChange.bind(this);
  }

  handleChange(event) {
    console.log("Saving change to a field...");

    const unsavedEntries = _.cloneDeep(this.props.appState.unsavedEntries);
    const unsavedEntry = unsavedEntries[this.props.appState.entryId];

    if (_.isObject(unsavedEntry)) {
      const fieldIndex = _.findIndex(unsavedEntry.fields, [
        "name",
        this.props.field.name
      ]);

      const updatedField = _.cloneDeep(this.props.field);
      updatedField.value = event.target.value;
      unsavedEntry.fields[fieldIndex] = updatedField;

      console.log("Setting updated unsavedEntries: ", unsavedEntries);
      this.props.setAppState({ unsavedEntries: unsavedEntries });
    } else {
      // TODO: Handle the case of user newly editing a persisted entry.
    }
  }

  render() {
    return (
      <div className="row">
        <div className="col-2">
          {/* TODO: Clicking the field name should copy the contents */}
          <button>{this.props.field.name}</button>
        </div>
        <div className="col">
          <input
            type="text"
            value={this.props.field.value}
            onChange={this.handleChange}
          />
        </div>
      </div>
    );
  }
}

Field.propTypes = {
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired,
  field: PropTypes.object.isRequired
};
