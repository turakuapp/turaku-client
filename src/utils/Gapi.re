[@bs.val] [@bs.scope "gapi"]
external load: (string, unit => unit) => unit = "load";

module Client = {
  type initOptions = {
    .
    "apiKey": string,
    "clientId": string,
    "discoveryDocs": array(string),
    "scope": string,
  };
  [@bs.val] [@bs.scope "gapi.client"]
  external init: initOptions => Js.Promise.t(unit) = "init";

  module Drive = {
    [@bs.module "./GapiJs"]
    external listFiles: unit => Js.Promise.t(array(string)) = "listFiles";
  };
};

module Auth2 = {
  type authInstance;
  type isSignedIn;

  [@bs.val] [@bs.scope "gapi.auth2"]
  external getAuthInstance: unit => authInstance = "getAuthInstance";

  [@bs.get] external isSignedIn: authInstance => isSignedIn = "isSignedIn";

  [@bs.send]
  external listenForIsSignedIn: (isSignedIn, bool => unit) => unit = "listen";

  [@bs.send] external getIsSignedIn: (isSignedIn, unit) => bool = "get";

  [@bs.send] external signIn: (authInstance, unit) => unit = "signIn";
  [@bs.send] external signOut: (authInstance, unit) => unit = "signOut";
};
