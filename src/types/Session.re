/* TODO: Figure out if we still need to track this. */
/* restorationAttempted: bool */

type invitations = list(Invitation.t);

type otherTeams = list(Team.t);

type currentTeam = (Team.id, Secrets.t);

type teams = (option(currentTeam), otherTeams);

type credentials = {
  token: string,
  encryptionHash: string,
};

type t =
  | SignedOut
  | SignedIn(credentials, teams, invitations);

let shouldRestore = (): bool => {
/* Dom.Storage.sessionStorage */
true
};

let restore = (session: t) => { () };