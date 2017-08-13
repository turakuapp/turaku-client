import _ from 'lodash'

export default class ApiService {
  constructor (token) {
    this.token = token
    this.baseUrl = 'http://turaku.dev/api/v0'
  }

  headers () {
    let defaultHeaders = {
      'Accept': 'application/json',
      'Content-Type': 'application/json'
    }

    if (this.token) {
      return _.merge(defaultHeaders, {Authorization: this.token})
    } else {
      return defaultHeaders
    }
  }

  parseResponse (response) {
    let promise = response.json()

    // TODO: Check whether the response was a 4XX or a 5XX. A 4XX should be parsed, and a 5XX should be 'handled' with a server-failure response.
    if (response.ok) {
      return promise
    } else {
      return promise.then(parsedResponse => Promise.reject(parsedResponse))
    }
  }

  fetch (path, method, body = null) {
    console.log('Calling ' + this.fullUrl(path))

    return fetch(this.fullUrl(path), {method: method, headers: this.headers(), body: body})
      .then(this.parseResponse)
  }

  get (path) {
    return this.fetch(path, 'GET')
  }

  post (path, params) {
    let body = JSON.stringify(params)
    return this.fetch(path, 'POST', body)
  }

  fullUrl (path) {
    return [this.baseUrl, path].join('/')
  }
}
