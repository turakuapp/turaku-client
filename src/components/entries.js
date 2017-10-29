import React from "react";
import PropTypes from "prop-types";
import "./entries.css";

export default class Entries extends React.Component {
  constructor(props) {
    super(props);

    this.state = {};

    this.addEntry = this.addEntry.bind(this);
  }

  addEntry() {
    console.log("Add a new entry...");
  }

  render() {
    return (
      <div className="row">
        <div className="col-3">
          <div className="entries__nav">
            <div className="pt-2">
              <input type="text" placeholder="Search" className="mr-2" />
              <button
                className="btn btn-primary btn-sm"
                onClick={this.addEntry}
              >
                Add new
              </button>
            </div>
          </div>
        </div>
        <div className="col-8">
          <p>Decrypted entry contents go here.</p>
        </div>
      </div>
    );
  }
}

Entries.propTypes = {
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired
};
