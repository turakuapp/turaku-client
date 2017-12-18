import React from "react";
import PropTypes from "prop-types";
import _ from "lodash";
import { WithContext as ReactTags } from "react-tag-input";
import HashService from "../services/hashService";
import CryptoService from "../services/cryptoService";

export default class EntryTags extends React.Component {
  constructor(props) {
    super(props);

    this.handleAddTag = this.handleAddTag.bind(this);
    this.handleDeleteTag = this.handleDeleteTag.bind(this);
    this.handleDragTag = this.handleDragTag.bind(this);
  }

  tagsForDisplay() {
    return _.map(this.props.entry.tags, tag => {
      if (_.isString(tag.name)) {
        return { text: tag.name };
      } else {
        return { id: tag.id, text: this.props.appState.tags[tag.id].name };
      }
    });
  }

  suggestedTags() {
    return _.map(this.props.appState.tags, tag => {
      return tag.name;
    });
  }

  async handleAddTag(tag) {
    // Create hash from tag text.
    const hashService = new HashService(
      tag.toLowerCase(),
      this.props.appState.team.password
    );

    const nameHash = await hashService.hexHash();

    // Check if client has tag in store.
    const savedTagId = _.findKey(this.props.appState.tags, tag => {
      return tag.nameHash === nameHash;
    });

    const savedTag = this.props.appState.tags[savedTagId];

    let newTag = null;

    if (_.isObject(savedTag)) {
      newTag = { id: savedTagId };
    } else {
      // when client does not have tag in store, encrypt tag text...
      const cryptoService = new CryptoService(
        this.props.appState.team.password,
        true
      );

      const encryptedTag = await cryptoService.encrypt(tag);
      newTag = { name: tag, encryptedName: encryptedTag, nameHash: nameHash };
    }

    // Now add the new tag to list of tags belonging to unsaved entry.
    const tagsClone = _.cloneDeep(this.props.entry.tags);
    tagsClone.push(newTag);

    this.saveChangeToTags(tagsClone);
  }

  handleDeleteTag(i) {
    // Remove the tag at supplied index.
    const tagsClone = _.cloneDeep(this.props.entry.tags);
    tagsClone.splice(i, 1);
    this.saveChangeToTags(tagsClone);
  }

  handleDragTag(tag, currPos, newPos) {
    const tagsClone = _.cloneDeep(this.props.entry.tags);

    // mutate array
    tagsClone.splice(currPos, 1);
    tagsClone.splice(newPos, 0, tag);

    // re-render
    this.saveChangeToTags(tagsClone);
  }

  saveChangeToTags(newTags) {
    const entryId = this.props.appState.entryId;
    const unsavedEntries = _.cloneDeep(this.props.appState.unsavedEntries);
    let unsavedEntry = unsavedEntries[entryId];

    if (!_.isObject(unsavedEntry)) {
      // The unsaved entry doesn't exist, copy it into the clone
      // of unsaved entries list from list of saved entries.
      unsavedEntry = _.cloneDeep(this.props.appState.entries[entryId]);
      unsavedEntries[entryId] = unsavedEntry;
    }

    unsavedEntry.tags = newTags;
    console.log("Setting updated unsavedEntries: ", unsavedEntries);
    this.props.setAppState({ unsavedEntries: unsavedEntries });
  }

  render() {
    return (
      <div className="row">
        <div className="col-2">Tags</div>
        <div className="col">
          <ReactTags
            tags={this.tagsForDisplay()}
            suggestions={this.suggestedTags()}
            handleDelete={this.handleDeleteTag}
            handleAddition={this.handleAddTag}
            handleDrag={this.handleDragTag}
          />
        </div>
      </div>
    );
  }
}

EntryTags.propTypes = {
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired,
  entry: PropTypes.object.isRequired
};
