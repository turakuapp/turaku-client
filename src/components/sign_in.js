import React from 'react'
import './sign_in.css'

import SignInService from '../services/sign_in_service.js'

export default class SignIn extends React.Component {
  constructor (props) {
    super(props)
    this.submit = this.submit.bind(this)
  }

  submit (event) {
    event.preventDefault()
    let email = document.getElementById('sign-in-email').value
    let password = document.getElementById('sign-in-password').value
    let signInService = new SignInService()

    signInService.signIn(email, password).then(token => {
      console.log('Signed in.')
      this.props.setAppState({ token: token })
    }).catch(exception => {
      // Handle invalid credentials / exception.
      console.log(exception, 'Sign in failed.')
    })
  }

  render () {
    return (
      <div className='container'>
        <div className='row justify-content-center' styleName='centered-container'>
          <div className='col-md-6 align-self-center'>
            <form onSubmit={this.submit}>
              <div className='form-group'>
                <label htmlFor='sign-in-email'>Email address</label>
                <input required type='email' className='form-control' id='sign-in-email' aria-describedby='emailHelp'
                  placeholder='Enter email' />
                <small id='emailHelp' className='form-text text-muted'>We'll never share your email with anyone else.
                </small>
              </div>

              <div className='form-group'>
                <label htmlFor='sign-in-password'>Password</label>
                <input required type='password' className='form-control' id='sign-in-password' placeholder='Password' />
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
