import React from "react";
import PropTypes from "prop-types";
import _ from "lodash";
import EyeIcon from "mdi-react/EyeIcon";
import EyeOffIcon from "mdi-react/EyeOffIcon";
import { WithContext as ReactTags } from "react-tag-input";

export default class Field extends React.Component {
  constructor(props) {
    super(props);

    this.state = { passwordVisible: false };

    this.handleChange = this.handleChange.bind(this);
    this.toggleVisibility = this.toggleVisibility.bind(this);
    this.handleAddTag = this.handleAddTag.bind(this);
    this.handleDeleteTag = this.handleDeleteTag.bind(this);
    this.handleDragTag = this.handleDragTag.bind(this);
  }

  handleChange(event) {
    console.log("Saving change to a text field...");

    this.saveChangeToField(event.target.value);
  }

  saveChangeToField(newValue) {
    const entryId = this.props.appState.entryId;
    const unsavedEntries = _.cloneDeep(this.props.appState.unsavedEntries);
    let unsavedEntry = unsavedEntries[entryId];

    if (!_.isObject(unsavedEntry)) {
      // The unsaved entry doesn't exist, copy it into the clone
      // of unsaved entries list from list of saved entries.
      unsavedEntry = _.cloneDeep(this.props.appState.entries[entryId]);
      unsavedEntries[entryId] = unsavedEntry;
    }

    const fieldIndex = _.findIndex(unsavedEntry.fields, [
      "name",
      this.props.field.name
    ]);

    const updatedField = _.cloneDeep(this.props.field);
    updatedField.value = newValue;
    unsavedEntry.fields[fieldIndex] = updatedField;

    console.log("Setting updated unsavedEntries: ", unsavedEntries);
    this.props.setAppState({ unsavedEntries: unsavedEntries });
  }

  isPassword() {
    return this.props.field.type === "password";
  }

  toggleVisibility() {
    this.setState({ passwordVisible: !this.state.passwordVisible });
  }

  fieldType() {
    if (this.props.field.type !== "password" || this.state.passwordVisible) {
      return "text";
    } else {
      return "password";
    }
  }

  suggestedTags() {
    return ["Tag 1", "Tag 2"];
  }

  handleDeleteTag(i) {
    const tagsClone = _.cloneDeep(this.props.field.value);
    tagsClone.splice(i, 1);
    this.saveChangeToField(tagsClone);
  }

  handleAddTag(tag) {
    const tagsClone = _.cloneDeep(this.props.field.value);

    tagsClone.push({
      id: tagsClone.length + 1,
      text: tag
    });

    this.saveChangeToField(tagsClone);
  }

  handleDragTag(tag, currPos, newPos) {
    const tagsClone = _.cloneDeep(this.props.field.value);

    // mutate array
    tagsClone.splice(currPos, 1);
    tagsClone.splice(newPos, 0, tag);

    // re-render
    this.saveChangeToField(tagsClone);
  }

  isTags() {
    return this.props.field.type === "tags";
  }

  render() {
    return (
      <div className="row">
        <div className="col-2">
          {/* TODO: Clicking the field name should copy the contents */}
          <button>{this.props.field.name}</button>
        </div>
        <div className="col">
          {!this.isTags() && (
            <input
              type={this.fieldType()}
              value={this.props.field.value}
              onChange={this.handleChange}
            />
          )}

          {this.isTags() && (
            <ReactTags
              tags={this.props.field.value}
              suggestions={this.suggestedTags()}
              handleDelete={this.handleDeleteTag}
              handleAddition={this.handleAddTag}
              handleDrag={this.handleDragTag}
            />
          )}

          {this.isPassword() && (
            <button
              className="ml-2 btn btn-sm btn-outline-secondary"
              onClick={this.toggleVisibility}
            >
              {this.state.passwordVisible && <EyeOffIcon />}
              {!this.state.passwordVisible && <EyeIcon />}
            </button>
          )}
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
