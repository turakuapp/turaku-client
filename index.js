import React from 'react'
import ReactDOM from 'react-dom'
import './src/style/index.scss'

ReactDOM.render(
  <div className="container">
    <div className="row justify-content-center" style={{height: '100vh', minHeight: '25rem'}}>
      <div className="col-md-6 align-self-center">
        <h1>Hello, world!</h1>
        <br/>
        <div className="m-1 turaku__container">This text is in an <code>m-1</code> <em>div</em>.</div>
      </div>
    </div>
  </div>,
  document.getElementById('turaku__root')
)
