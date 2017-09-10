import React from "react";
import PropTypes from "prop-types";

export default class SignUp extends React.Component {
  constructor(props) {
    super(props);
    this.signIn = this.signIn.bind(this);
  }

  submit(event) {
    event.preventDefault();
  }

  signIn(event) {
    event.preventDefault();
    this.props.setAppState({ page: "signIn" });
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

              <button
                className="mt-2 ml-md-2 btn btn-secondary"
                onClick={this.signIn}
              >
                Sign In
              </button>
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
