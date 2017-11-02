import React from "react";
import PropTypes from "prop-types";

export default class Field extends React.Component {
  constructor(props) {
    super(props);

    this.handleChange = this.handleChange.bind(this);
  }

  handleChange(event) {
    console.log("A field has changed...");
  }

  render() {
    return (
      <div className="row">
        <div className="col-3">
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
