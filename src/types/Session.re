type otherTeams = list(Team.t);

type currentTeam = (Team.id, Secrets.t);

type teams = (option(currentTeam), otherTeams);

type credentials = {
  token: string,
  encryptionHash: string,
};

type t =
  | SignedOut
  | SignedIn(credentials, teams);

let shouldRestore = () : bool => true;

/* Dom.Storage.sessionStorage */
let restore = (session: t) => ();