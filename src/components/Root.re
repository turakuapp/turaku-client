[%bs.raw {|require("@fortawesome/fontawesome-free/js/all.js")|}];

let str = React.string;

let signOut = (setSession, ()) => setSession(_ => None);

let updateSigninStatus = isSignedIn => {
  Js.log2("isSignedIn: ", isSignedIn);

  if (isSignedIn) {
    Gapi.Client.Drive.listFiles()
    |> Js.Promise.then_(files => Js.log(files) |> Js.Promise.resolve)
    |> Js.Promise.catch(error =>
         Js.log2("updateSigninStatus error:", error) |> Js.Promise.resolve
       )
    |> ignore;
  };
};

let initClient = () => {
  Gapi.Client.init({
    "apiKey": Env.apiKey,
    "clientId": Env.clientId,
    "discoveryDocs": [|Env.discoveryDocs|],
    "scope": Env.scopes,
  })
  |> Js.Promise.then_(() => {
       let authInstance = Gapi.Auth2.getAuthInstance();
       authInstance
       ->Gapi.Auth2.isSignedIn
       ->Gapi.Auth2.listenForIsSignedIn(updateSigninStatus);
       authInstance->Gapi.Auth2.isSignedIn->Gapi.Auth2.getIsSignedIn()
       |> updateSigninStatus;
       Js.Promise.resolve();
     })
  |> Js.Promise.catch(error =>
       Js.log2("initClient error:", error) |> Js.Promise.resolve
     )
  |> ignore;
};

let handleClientLoad = _ => {
  Js.log("handleClientLoad has been called");
  Gapi.load("client:auth2", initClient);
};

[@react.component]
let make = () => {
  let (session, setSession) = React.useState(() => None);
  let (logs, setLogs) = React.useState(() => [||]);

  let log = message => setLogs(logs => logs |> Js.Array.concat([|message|]));
  let signOut = () => setSession(_ => None);

  let signIn = (session, encryptedTeams) => {
    setSession(_ => Some(session));
  };

  <div className="root">
    {switch (session) {
     | None => <SignInMenu log signIn />
     /* | Some(session) => <SignedInRoot log session signOut /> */
     | Some(session) => <div> {"Signed in" |> str} </div>
     }}
    /* Also show logs at the bottom, somehow. */
    <div>
      {logs
       |> Array.map(logEntry => <span> {logEntry |> str} </span>)
       |> React.array}
    </div>
    <button className="mt-2 border rounded-lg p-2" onClick=handleClientLoad>
      {"Load client" |> str}
    </button>
  </div>;
};
