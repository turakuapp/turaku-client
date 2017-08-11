const path = require('path')
const context = path.resolve(__dirname, 'src')

module.exports = {
  entry: './index.js',
  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: 'bundle.js'
  },
  module: {
    loaders: [
      {
        test: /\.js$/,
        exclude: /node_modules/,
        use: [{
          loader: 'babel-loader',
          query: {
            plugins: [
              'react-css-modules'
            ]
          }
        }]
      }, {
        test: /\.css$/,
        include: context,
        use: [{
          loader: 'style-loader' // Creates style nodes from JS strings.
        }, {
          loader: 'css-loader', // Translates CSS into CommonJS.
          options: {
            importLoaders: 1,
            modules: true,
            localIdentName: '[path]___[name]__[local]___[hash:base64:5]'
          }
        }]
      }, {
        test: /\.scss$/,
        use: [{
          loader: 'style-loader' // Creates style nodes from JS strings.
        }, {
          loader: 'css-loader',
        }, {
          loader: 'sass-loader', // Translates SCSS into CommonJS.
        }]
      }
    ]
  }
}
