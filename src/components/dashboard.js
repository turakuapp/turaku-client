import React from 'react'

import './dashboard.css'

export default class Dashboard extends React.Component {
  render () {
    return (
      <div className='container-fluid'>
        <div className='row'>
          <div className='col' styleName='tags'>
            Tags
          </div>

          <div className='col-10' styleName='entries'>
            Entries
          </div>
        </div>
      </div>
    )
  }
}

Dashboard.propTypes = {
  appState: React.PropTypes.object,
  setAppState: React.PropTypes.func
}
