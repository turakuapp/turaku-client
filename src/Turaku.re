type selectable =
  | NothingSelected
  | EntrySelected(Entry.id);

type page =
  | SignUpPage
  | SignInPage
  | DashboardPage(selectable)
  | TeamSelectionPage
  | LoadingPage;

type action =
  | SignUp
  | SignIn(
      AccessToken.t,
      list(Team.t),
      list(Invitation.t),
      EncryptionHash.t,
    )
  | Navigate(page)
  | SkipLoading
  | CreateTeam(Team.t);

type flags = {justSignedUp: bool};

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
  currentPage: LoadingPage,
  flags: {
    justSignedUp: false,
  },
  invitations: [],
  teams: [],
  currentTeam: None,
  entries: [],
  unsavedEntries: [],
};

/* TODO: COMPLICATED REFACTOR: The SignIn action accepts token and encryption hash, when it should be accepting the session, BUT only a signed in session. */
/* https://stackoverflow.com/questions/24653301/accepting-only-one-variant-of-sum-type-as-ocaml-function-parameter */
let reducer = (action, state) =>
  switch (action) {
  | SignIn(token, teams, invitations, encryptionHash) =>
    ReasonReact.Update({
      ...state,
      currentPage: TeamSelectionPage,
      session: Session.create(token, encryptionHash),
      teams,
      invitations,
      flags: {
        justSignedUp: false,
      },
    })
  | SignUp =>
    ReasonReact.Update({
      ...state,
      currentPage: SignInPage,
      flags: {
        justSignedUp: true,
      },
    })
  | Navigate(destination) =>
    ReasonReact.Update({...state, currentPage: destination})
  | SkipLoading => ReasonReact.Update({...state, currentPage: SignInPage})
  | CreateTeam(team) =>
    ReasonReact.Update({
      ...state,
      teams: [team, ...state.teams],
      currentTeam: Some(team),
    })
  };