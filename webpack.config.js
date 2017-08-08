const path = require('path')

module.exports = {
  entry: './index.js',
  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: 'bundle.js'
  },
  module: {
    loaders: [
      {
        test: /\.jsx?$/,
        use: ['babel-loader'],
        exclude: /node_modules/
      }, {
        test: /\.scss$/,
        use: [{
          loader: 'style-loader' // Creates style nodes from JS strings.
        }, {
          loader: 'css-loader' // Translates CSS into CommonJS.
        }, {
          loader: 'postcss-loader' // Use PostCSS plugins.
        }, {
          loader: 'sass-loader' // Compiles Sass to CSS.
        }]
      }
    ]
  }
}
