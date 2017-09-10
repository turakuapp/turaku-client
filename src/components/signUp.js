import React from "react";
import PropTypes from "prop-types";
import { Link } from "react-router-dom";

export default class SignUp extends React.Component {
  submit(event) {
    event.preventDefault();
  }

  render() {
    return (
      <div className="container">
        <div className="row justify-content-center sign-in__centered-container">
          <div className="col-md-6 align-self-center">
            <form onSubmit={this.submit}>
              <div className="form-group">
                <label htmlFor="sign-up-form__email">Email address</label>
                <input
                  type="email"
                  className="form-control"
                  id="sign-up-form__email"
                  aria-describedby="emailHelp"
                  placeholder="Enter email"
                />

                <small id="emailHelp" className="form-text text-muted">
                  We'll never share your email with anyone else.
                </small>
              </div>

              <div className="form-group">
                <label htmlFor="sign-up-form__password">Password</label>
                <input
                  type="password"
                  className="form-control"
                  id="sign-up-form__password"
                  placeholder="Password"
                />
              </div>

              <div className="form-group">
                <label htmlFor="sign-up-form__password-confirmation">
                  Confirm Password
                </label>
                <input
                  type="password"
                  className="form-control"
                  id="sign-up-form__password-confirmation"
                  placeholder="Password"
                />
              </div>

              <button type="submit" className="mt-2 btn btn-primary">
                Submit
              </button>

              <Link to="/sign_in">
                <button className="mt-2 ml-md-2 btn btn-secondary">
                  Sign In
                </button>
              </Link>
            </form>
          </div>
        </div>
      </div>
    );
  }
}

SignUp.propTypes = {
  appState: PropTypes.object,
  setAppState: PropTypes.func
};
