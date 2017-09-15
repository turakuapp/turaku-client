import React from "react";
import PropTypes from "prop-types";
import SignInService from "../services/signInService";
import "./signIn.css";
import { Link } from "react-router-dom";

export default class SignIn extends React.Component {
  constructor(props) {
    super(props);
    this.submit = this.submit.bind(this);
  }

  componentWillMount() {
    if (this.props.appState.justSignedUp) {
      this.props.setAppState({ justSignedUp: false, signedUp: true });
    }
  }

  submit(event) {
    event.preventDefault();
    let email = document.getElementById("sign-in-email").value;
    let password = document.getElementById("sign-in-password").value;
    let signInService = new SignInService();

    signInService
      .signIn(email, password)
      .then(token => {
        console.log("Signed in.");
        this.props.setAppState({ token: token });
      })
      .catch(exception => {
        // Handle invalid credentials / exception.
        console.log(exception, "Sign in failed.");
      });
  }

  render() {
    return (
      <div className="container">
        <div className="row justify-content-center sign-in__centered-container">
          <div className="col-md-6 align-self-center">
            {this.props.appState.signedUp && (
              <div className="alert alert-success" role="alert">
                Thank you for signing up! Please confirm your email address
                before signing in.
              </div>
            )}

            <form onSubmit={this.submit}>
              <div className="form-group">
                <label htmlFor="sign-in-email">Email address</label>
                <input
                  required
                  type="email"
                  className="form-control"
                  id="sign-in-email"
                  aria-describedby="emailHelp"
                  placeholder="Your registered email address"
                />
              </div>

              <div className="form-group">
                <label htmlFor="sign-in-password">Password</label>
                <input
                  required
                  type="password"
                  className="form-control"
                  id="sign-in-password"
                  placeholder="Password"
                />
              </div>

              <button type="submit" className="mt-2 btn btn-primary">
                Sign In
              </button>

              <Link to="/sign_up">
                <button className="mt-2 ml-md-2 btn btn-secondary">
                  Sign Up
                </button>
              </Link>
            </form>
          </div>
        </div>
      </div>
    );
  }
}

SignIn.propTypes = {
  appState: PropTypes.object,
  setAppState: PropTypes.func
};
