# Turaku Client

## Demo

Here's a short demo of the (WIP) client!

![Turaku Demo](https://www.turaku.com/uploads/static/turaku_demo_v1.gif)

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

2. Fetch latest GraphQL schema from Turaku Server:

```
yarn send-introspection-query http://turaku.localhost/graphql
```

4. Compile ReasonML code to Javascript:

```
yarn run start
```

5. Finally, run the server.

```
yarn run server
```

This should be all that is required to boot up the server. Visit http://localhost:PORT (check server logs) to use the application. As noted above, Turaku server should be online for the app to work properly.
