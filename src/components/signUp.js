import React from "react";
import PropTypes from "prop-types";
import { Link, Redirect } from "react-router-dom";
import SignUpService from "../services/signUpService";

export default class SignUp extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      signUpComplete: false
    };

    this.submit = this.submit.bind(this);
  }

  submit(event) {
    event.preventDefault();

    let name = document.getElementById("sign-up-form__name").value;
    let email = document.getElementById("sign-up-form__email").value;
    let password = document.getElementById("sign-up-form__password").value;

    // TODO: Compare the confirmation against the supplied password.
    // let passwordConfirmation = document.getElementById(
    //   "sign-up-form__password-confirmation"
    // ).value;

    let signUpService = new SignUpService();
    let that = this;

    signUpService
      .signUp(name, email, password)
      .then(() => {
        that.props.setAppState(
          {
            redirectFrom: "SignUp",
            redirectTo: "SignIn"
          },
          () => {
            that.setState({ signUpComplete: true });
          }
        );
      })
      .catch(exception => {
        // Handle exception.
        console.log(exception, "Sign up failed. <shrug>");
      });
  }

  render() {
    if (this.state.signUpComplete === true) {
      return <Redirect to="/sign_in" />;
    }

    return (
      <div className="container">
        <div className="row justify-content-center sign-in__centered-container">
          <div className="col-md-6 align-self-center">
            <form onSubmit={this.submit}>
              <div className="form-group">
                <label htmlFor="sign-up-form__name">Name</label>
                <input
                  className="form-control"
                  id="sign-up-form__name"
                  placeholder="What should we call you?"
                />
              </div>

              <div className="form-group">
                <label htmlFor="sign-up-form__email">Email address</label>
                <input
                  type="email"
                  className="form-control"
                  id="sign-up-form__email"
                  aria-describedby="emailHelp"
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
                  placeholder="Again, to be sure"
                />
                <small id="emailHelp" className="form-text text-muted">
                  Please store this password in your personal password manager.
                  The Turaku team cannot recover your data if you forget your
                  password.{" "}
                  <a href="help.turaku.com/security">
                    Read about Turaku's security
                  </a>{" "}
                  to learn more.
                </small>
              </div>

              <button type="submit" className="mt-2 btn btn-primary">
                Submit
              </button>

              <Link to="/sign_in">
                <button className="mt-2 ml-2 btn btn-secondary">Sign In</button>
              </Link>
            </form>
          </div>
        </div>
      </div>
    );
  }
}

SignUp.propTypes = {
  appState: PropTypes.object.isRequired,
  setAppState: PropTypes.func.isRequired
};
