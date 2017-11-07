import React from "react";
import PropTypes from "prop-types";

export default class EntryChoice extends React.Component {
  render() {
    return <div key={this.props.entryId}>Title: {this.props.entry.title}</div>;
  }
}

EntryChoice.propTypes = {
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired,
  entryId: PropTypes.string.isRequired,
  entry: PropTypes.object.isRequired
};
