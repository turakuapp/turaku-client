import React from 'react'
import './sign_in.css'

export default class SignIn extends React.Component {
  constructor (props) {
    super(props)
    this.handleSubmit = this.handleSubmit.bind(this)
  }

  handleSubmit (event) {
    event.preventDefault()

    this.props.setAppState({
      email: document.getElementById('sign-in-email').value,
      password: document.getElementById('sign-in-password').value,
      rememberMe: document.getElementById('sign-in-remember-me').checked
    })
  }

  render () {
    return (
      <div className='container'>
        <div className='row justify-content-center' styleName='centered-container'>
          <div className='col-md-6 align-self-center'>
            <form onSubmit={this.handleSubmit}>
              <div className='form-group'>
                <label htmlFor='sign-in-email'>Email address</label>
                <input type='email' className='form-control' id='sign-in-email' aria-describedby='emailHelp'
                  placeholder='Enter email' />
                <small id='emailHelp' className='form-text text-muted'>We'll never share your email with anyone else.
                </small>
              </div>

              <div className='form-group'>
                <label htmlFor='sign-in-password'>Password</label>
                <input type='password' className='form-control' id='sign-in-password' placeholder='Password' />
              </div>

              <div className='form-check'>
                <label className='form-check-label'>
                  <input id='sign-in-remember-me' type='checkbox' className='form-check-input' />
                  Remember me
                </label>
              </div>
              <button type='submit' className='mt-2 btn btn-primary'>Sign In</button>
            </form>
          </div>
        </div>
      </div>
    )
  }
}

SignIn.propTypes = {
  appState: React.PropTypes.object,
  setAppState: React.PropTypes.func
}
