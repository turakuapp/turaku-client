import React from "react";

export default class SignUp extends React.Component {
  submit(event) {
    event.preventDefault();
  }

  render() {
    return (
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

        <button type="submit" className="btn btn-primary">
          Submit
        </button>
      </form>
    );
  }
}

SignUpForm.propTypes = {};
