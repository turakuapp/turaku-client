type selectable =
  | NothingSelected
  | EntrySelected(Entry.id);

type page =
  | SignUpPage
  | SignInPage
  | DashboardPage(selectable);

type action =
  | SignedUp
  | SignedIn(string, list(Team.t), list(Invitation.t), string)
  | Navigate(page);

type flags = {
  restorationAttempted: bool,
  justSignedUp: bool,
};

type teams = list(Team.t);

type entries = list(Entry.t);

type unsavedEntries = list(Entry.t);

type state = {
  session: Session.t,
  currentPage: page,
  flags,
  invitations: list(Invitation.t),
  teams,
  currentTeam: option(Team.t),
  entries,
  unsavedEntries,
};

let initialState = {
  session: Session.signOut(),
  currentPage: SignInPage,
  flags: {
    restorationAttempted: true,
    justSignedUp: false,
  },
  invitations: [],
  teams: [],
  currentTeam: None,
  entries: [],
  unsavedEntries: [],
};

let reducer = (action, state) =>
  switch (action) {
  | SignedIn(token, teams, invitations, encryptionHash) =>
    ReasonReact.Update({
      ...state,
      session: Session.signIn(~token, ~encryptionHash),
      teams,
      invitations,
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
  | Navigate(destination) =>
    ReasonReact.Update({...state, currentPage: destination})
  };