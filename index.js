import React from 'react'
import ReactDOM from 'react-dom'
import './src/style/index.scss'

ReactDOM.render(
  <div>
    <h1>Hello, world!</h1>
    <div className="m-1 turaku__container">This text is in an <code>m-1</code> <em>div</em>.</div>
  </div>,
  document.getElementById('turaku__root')
)
