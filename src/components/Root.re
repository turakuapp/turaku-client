[%bs.raw {|require("@fortawesome/fontawesome-free/js/all.js")|}];

let str = React.string;

/* let currentComponent = (state, send) =>
   switch (state) {
   | Turaku.SignedOutUser(page) =>
     switch (page) {
     | LoadingPage => <LoadingScreen appSend=send />
     | SignInPage(data) => <SignInMenu data appSend=send />
     | SignUpPage => <SignUpMenu appSend=send />
     }
   | SignedInUser(userData) =>
     switch (userData.teams |> SelectableList.selected) {
     | None =>
       <TeamSelection
         session={userData.session}
         invitations={userData.invitations}
         teams={userData.teams}
         appSend=send
       />
     | Some(team) =>
       <Dashboard
         session={userData.session}
         team
         dashboardMenu={userData.dashboardMenu}
         appSend=send
       />
     }
   }; */

let signOut = (setSession, ()) => setSession(_ => None);

[@react.component]
let make = () => {
  let (session, setSession) = React.useState(() => None);
  let (logs, setLogs) = React.useState(() => [||]);

  let log = message => setLogs(logs => Js.Array.push(message) |> ignore);
  let signOut = () => setSession(_ => None);
  let signIn = session => setSession(_ => Some(session));

  <div>
    {
      switch (session) {
      | Some(session) => <UserDashboard session signOut />
      | None => <SignInMenu log signIn />
      }
    }

    /* Also show logs at the bottom, somehow. */
    <div> {"Logs go here" |> str}
  </div>;
};
