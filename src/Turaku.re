type selectable =
  | NothingSelected
  | EntrySelected(Entry.id);

type page =
  | SignUpPage
  | SignInPage
  | DashboardPage(selectable)
  | TeamSelectionPage;

type action =
  | SignedUp
  | SignedIn(
      AccessToken.t,
      list(Team.t),
      list(Invitation.t),
      EncryptionHash.t,
    )
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
  session: Session.signedOut(),
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
      currentPage: TeamSelectionPage,
      session: Session.create(~token, ~encryptionHash),
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