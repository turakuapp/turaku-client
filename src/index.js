import React from 'react'
import ReactDOM from 'react-dom'
import './style/index.css'
import 'bootstrap/scss/bootstrap.scss'

import AppState from './components/app_state.js'
import App from './components/app.js'

ReactDOM.render(
  <AppState>
    <App />
  </AppState>,
  document.getElementById('turaku__root')
)
