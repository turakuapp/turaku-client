# Turaku Client

## Development Setup

Install all packages, build CSS with Tailwind, and run the `start` script.

```
yarn install
yarn run tailwind build src/tailwind/index.css -o src/index.css
yarn run start
```

This should be all that is required to boot up the application in your browser. The API (unreleased) should be online for the app to work properly.

In some cases, you may also need to tell Bucklescript to watch and build ReasonML files manually, before using the `start` script listed above:

```
yarn run bsb -make-world -w
```

## Development Status

- ReactJS Prototype: **Completed in Dec 2017**
- ReasonML + GraphQL conversion: **Completed in July 2018** ([Github issue](https://github.com/turakuapp/turaku-client/issues/1)).
- Private Alpha: **Check [Alpha milestone](https://github.com/turakuapp/turaku-client/milestone/1).**
- User permissions implementation
- Public Beta

## Dev Setup

### Execution

1.  `yarn start` to run webpack-dev-server.
2.  `yarn run dev` to load the application inside Electron.

## Resources used to set up this project

- This project was bootstrapped with [Create React App](https://github.com/facebookincubator/create-react-app).
- Mixing CRA and Electron without Ejecting: [FreeCodeCamp Article by Christian Sepulveda](https://medium.freecodecamp.org/building-an-electron-application-with-create-react-app-97945861647c).
