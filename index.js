import React from 'react'
import ReactDOM from 'react-dom'
import './src/style/index.css'
import 'bootstrap/scss/bootstrap.scss'

import AppState from './src/components/app_state.js'
import SignIn from './src/components/sign_in.js'

ReactDOM.render(
  <AppState>
    <SignIn />
  </AppState>,
  document.getElementById('turaku__root')
)
