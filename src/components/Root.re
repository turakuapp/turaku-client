[%bs.raw {|require("@fortawesome/fontawesome-free/js/all.js")|}];

open Turaku;

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

module EncryptedTeam = {
  type t;
};

module EncryptedEntry = {
  type t = {teamId: Team.id};

  let teamId = t => t.teamId;
};

type state = {
  session: option(Session.t),
  teams: list(EncryptedTeam.t),
  entries: list(EncryptedEntry.t),
};

/* This should load encrypted team and entry data from local persistent storage. */
let initialState = {session: None, teams: [], entries: []};

type action =
  | SignIn(Session.t)
  | SignOut
  | UpdateEntries(list(EncryptedEntry.t), Team.id)
  | AddEntry(EncryptedEntry.t)
  | ReplaceEntry(EncryptedEntry.t)
  | DeleteEntry(EncryptedEntry.t)
  | UpdateTeams(list(Team.t))
  | AddTeam(Team.t)
  | ReplaceTeam(Team.t)
  | DeleteTeam(Team.t);

let reducer = (state, action) =>
  switch (action) {
  | SignIn(session) => {...state, session: Some(session)}
  | SignOut => {...state, session: None}
  | UpdateEntries(upToDateEntries, teamId) => {
      ...state,
      entries:
        state.entries
        |> List.filter(entry => entry |> EncryptedEntry.teamId != teamId)
        |> List.rev_append(upToDateEntries),
    }
  | AddEntry(encryptedEntry) => state
  | ReplaceEntry(encryptedEntry) => state
  | DeleteEntry(encryptedEntry) => state
  | UpdateTeams(upToDateTeams) => state
  | AddTeam(team) => state
  | ReplaceTeam(team) => state
  | DeleteTeam(team) => state
  };

[@react.component]
let make = () => {
  /* let (state, send) = React.useReducer(Turaku.reducer, Turaku.initialState); */
  let (state, send) = React.useReducer(reducer, initialState);

  switch (state.session) {
  | Some(session) =>
    /* User has signed in. */
    <div> {"Signed in" |> str} </div>
  | None =>
    /* User hasn't signed in yet. */
    <div> {"Sign in" |> str} </div>
  };
};
