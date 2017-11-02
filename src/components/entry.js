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
    return _.isObject(this.props.appState.entry);
  }

  handleTitleChange(event) {
    console.log("Title has changed...");
  }

  entryData() {
    // {
    //   title: "New Entry",
    //   fields: [
    //     { name: "user", type: "user", value: "" },
    //     { name: "password", type: "password", value: "" },
    //     { name: "tags", type: "tags", value: "" }
    //   ]
    // };

    const entry = this.props.appState.entry;

    return (
      <div>
        <input
          type="text"
          value={entry.title}
          onChange={this.handleTitleChange}
        />
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
