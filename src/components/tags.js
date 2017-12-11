import React from "react";
import PropTypes from "prop-types";
import { Link } from "react-router-dom";
import ListTagsService from "../services/tags/listService";
import CryptoService from "../services/cryptoService";
import _ from "lodash";

export default class Tags extends React.Component {
  componentDidMount() {
    const listService = new ListTagsService(
      this.props.appState.token,
      this.props.appState.team.id
    );

    listService.list().then(encryptedTags => {
      for (const encryptedTag of encryptedTags) {
        this.decryptAndStoreTag(encryptedTag);
      }
    });
  }

  async decryptAndStoreTag(tag) {
    const cryptoService = new CryptoService(
      this.props.appState.team.password,
      true
    );

    const name = await cryptoService.decrypt(tag.encrypted_name);
    const tagsClone = _.cloneDeep(this.props.appState.tags);

    tagsClone[tag.id] = name;

    this.props.setAppState({ tags: tagsClone });
  }

  savedTags() {
    if (_.isEmpty(this.props.appState.tags)) {
      return;
    }

    return _.map(this.props.appState.tags, tag => {
      return <span>{tag.name}</span>;
    });
  }

  render() {
    return (
      <div>
        <h3>Tags</h3>

        <div>
          <Link to="/dash/entries/all">All tags</Link>
          {this.savedTags()}
        </div>
      </div>
    );
  }
}

Tags.propTypes = {
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired
};
