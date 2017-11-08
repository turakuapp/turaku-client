import React from "react";
import PropTypes from "prop-types";
import _ from "lodash";
import "./entryChoice.css";

export default class EntryChoice extends React.Component {
  constructor(props) {
    super(props);

    this.chooseEntry = this.chooseEntry.bind(this);
  }

  title() {
    if (
      _.isString(this.props.entry.title) &&
      this.props.entry.title.length > 0
    ) {
      return this.props.entry.title;
    } else {
      return <em>Nameless entry</em>;
    }
  }

  chooseEntry() {
    if (this.props.entryId !== this.props.appState.entryId) {
      this.props.setAppState({ entryId: this.props.entryId });
    }
  }

  blockClasses() {
    const classes = "mr-3 mt-2 p-2 entry-choice";

    if (this.props.entryId === this.props.appState.entryId) {
      return classes + " entry-choice--chosen";
    } else {
      return classes;
    }
  }

  render() {
    return (
      <div
        key={this.props.entryId}
        className={this.blockClasses()}
        onClick={this.chooseEntry}
      >
        {this.title()}
      </div>
    );
  }
}

EntryChoice.propTypes = {
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired,
  entryId: PropTypes.string.isRequired,
  entry: PropTypes.object.isRequired
};
