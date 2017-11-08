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

    const staleEntries = _.cloneDeep(this.props.appState.staleEntries);
    const staleEntry = staleEntries[this.props.appState.entryId];

    if (_.isObject(staleEntry)) {
      const fieldIndex = _.findIndex(staleEntry.fields, [
        "name",
        this.props.field.name
      ]);

      const updatedField = _.cloneDeep(this.props.field);
      updatedField.value = event.target.value;
      staleEntry.fields[fieldIndex] = updatedField;

      console.log("Setting updated staleEntries: ", staleEntries);
      this.props.setAppState({ staleEntries: staleEntries });
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
