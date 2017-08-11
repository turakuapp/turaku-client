import React from 'react'
import './sign_in.css'

export default class SignIn extends React.Component {
  render () {
    return (
      <div className="container">
        <div className="row justify-content-center" styleName='centered-container'>
          <div className="col-md-6 align-self-center">
            <h1>Hello, world!</h1>
            <br/>
            <div className="m-1 turaku__container">This text is in an <code>m-1</code> <em>div</em>.</div>
          </div>
        </div>
      </div>
    )
  }
}
