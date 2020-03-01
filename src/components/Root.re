[%bs.raw {|require("@fortawesome/fontawesome-free/js/all.js")|}];

let str = React.string;

let signOut = (setSession, ()) => setSession(_ => None);

[@react.component]
let make = () => {
  let (session, setSession) = React.useState(() => None);
  let (logs, setLogs) = React.useState(() => [||]);

  let log = message => setLogs(logs => logs |> Js.Array.concat([|message|]));
  let signOut = () => setSession(_ => None);

  let signIn = (session, encryptedTeams) => {
    setSession(_ => Some(session));
  };

  <div>
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
  </div>;
};
