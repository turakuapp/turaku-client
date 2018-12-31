# Turaku Client

## Demo

Here's a (very) short demo of the client, in action! Click the image to watch on YouTube.

[![Watch the video](https://img.youtube.com/vi/UwLE9S5iwyA/maxresdefault.jpg)](https://youtu.be/UwLE9S5iwyA)

## Development Status

- [x] ReactJS Prototype: **Completed in Dec 2017**
- [x] ReasonML + GraphQL conversion: **Completed in July 2018** ([Github issue](https://github.com/turakuapp/turaku-client/issues/1)).
- [ ] Private Alpha: **Check [Alpha milestone](https://github.com/turakuapp/turaku-client/milestone/1).**
- [ ] User permissions implementation
- [ ] Public Beta

## Development Setup

### Requirements

1. [Turaku server](https://github.com/turakuapp/turaku-server) should be running on your system, [assumed to be accessible](https://github.com/turakuapp/turaku-client/blob/master/src/utils/Api.re#L3) at `turaku.localhost`. See the server repository's [README](https://github.com/turakuapp/turaku-server/blob/master/README.md) for more details.
2. A working ReasonML development environment - see [official instructions](https://reasonml.github.io/docs/en/installation).

### Steps

1. Install all packages:

```
yarn install
```

2. Build CSS with Tailwind:

```
yarn run tailwind build src/tailwind/index.css -o src/index.css
```

3. Fetch latest GraphQL schema from Turaku Server:

```
yarn send-introspection-query http://turaku.localhost/graphql
```

4. Compile ReasonML code to Javascript:

```
yarn run bsb -make-world
```

5. Finally, run the `start` script.

```
yarn run start
```

This should be all that is required to boot up the application in your browser. As noted above, Turaku server should be online for the app to work properly.

You may also want to tell Bucklescript to watch and build JS whenever Reason code is updated. Add a `-w` flag to the `bsb` command above to do so.

## Resources used to set up this project

- This project was bootstrapped with [Create React App](https://github.com/facebookincubator/create-react-app).
- Mixing CRA and Electron without Ejecting: [FreeCodeCamp Article by Christian Sepulveda](https://medium.freecodecamp.org/building-an-electron-application-with-create-react-app-97945861647c).
