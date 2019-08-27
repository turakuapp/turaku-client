let str = React.string;

[@react.component]
let make = (~log, ~session, ~signOut) => {
  let (teamId, setTeamId) = React.useState(() => None);

  switch (teamId) {
  | None => <TeamSelection log session />
  | Some(teamId) => <Dashboard log session signOut teamId />
  };
};
