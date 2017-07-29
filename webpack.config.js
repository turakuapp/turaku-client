const path = require('path');

module.exports = {
  entry: './main.js',
  output: {
    path: path.resolve('dist'),
    filename: 'turaku.js'
  },
  module: {
    loaders: [
      { test: /\.js$/, loader: 'babel-loader', exclude: /node_modules/ },
      { test: /\.jsx$/, loader: 'babel-loader', exclude: /node_modules/ }
    ]
  }
}
