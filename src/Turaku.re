type action =
  | SignedUp
  | SignedIn;

type selectable =
  | NothingSelected
  | EntrySelected(Entry.id);

type page =
  | SignUpPage
  | SignInPage
  | DashboardPage(selectable);

type flags = {
  restorationAttempted: bool,
  justSignedUp: bool,
};

type otherTeams = list(Team.t);

type currentTeam = (Team.id, Secrets.t);

type teams = (option(currentTeam), otherTeams);

type entries = list(Entry.t);

type unsavedEntries = list(Entry.t);

type state = {
  session: Session.t,
  currentPage: page,
  flags,
  invitations: list(Invitation.t),
  teams,
  entries,
  unsavedEntries,
};

let initialState = {
  session: Session.SignedOut,
  currentPage: SignUpPage,
  flags: {
    restorationAttempted: true,
    justSignedUp: false,
  },
  invitations: [],
  teams: (None, []),
  entries: [],
  unsavedEntries: [],
};

let reducer = (action, state) =>
  switch (action) {
  | SignedIn =>
    ReasonReact.Update({
      ...state,
      flags: {
        ...state.flags,
        justSignedUp: false,
      },
    })
  | SignedUp =>
    ReasonReact.Update({
      ...state,
      currentPage: SignInPage,
      flags: {
        ...state.flags,
        justSignedUp: true,
      },
    })
  };