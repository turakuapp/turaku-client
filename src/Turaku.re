type action =
  | AttemptRestoration
  | SignedUp
  | SignedIn;

type credentials = {
  token: string,
  encryptionHash: string,
};

type teamId = int;

type team = {
  id: teamId,
  name: string,
  /* and more */
};

type entries;

type unsavedEntries;

type tags;

type secrets = {
  entries,
  unsavedEntries,
  tags,
};

type currentTeam = (teamId, secrets);

type otherTeams = list(team);

type teams = (option(currentTeam), otherTeams);

type invitations;

type session =
  | None
  | SignedIn(credentials, teams, invitations);

type page =
  | SignUp
  | SignIn
  | Dashboard;

type state = {
  session,
  restorationAttempted: bool,
  currentPage: page,
};