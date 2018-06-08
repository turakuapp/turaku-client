type selectable =
  | NothingSelected
  | EntrySelected(Entry.id);

type selectedTeamId = Team.id;

type page =
  | SignUpPage
  | SignInPage
  | DashboardPage(SelectedTeam.t, selectable)
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
  | CreateTeam(Team.t, TeamPassword.t)
  | SelectTeam(Team.t, TeamPassword.t)
  | SignOut;

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
  entries,
  unsavedEntries,
};

let initialState = {
  session: Session.signedOut(),
  currentPage: LoadingPage,
  flags: {
    justSignedUp: false,
  },
  invitations: [],
  teams: [],
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
  | SelectTeam(team, teamPassword) =>
    ReasonReact.Update({
      ...state,
      currentPage:
        DashboardPage(
          team |> Team.getId |> SelectedTeam.create(teamPassword),
          NothingSelected,
        ),
    })
  | CreateTeam(team, teamPassword) =>
    ReasonReact.Update({
      ...state,
      teams: [team, ...state.teams],
      currentPage:
        DashboardPage(
          team |> Team.getId |> SelectedTeam.create(teamPassword),
          NothingSelected,
        ),
    })
  | SignOut =>
    ReasonReact.Update({
      ...state,
      session: state.session |> Session.signOut,
      currentPage: SignInPage,
    })
  };

let selectedTeam = s =>
  switch (s.currentPage) {
  | DashboardPage(selectedTeam, _) =>
    s.teams
    |> List.find(t => t |> Team.getId == (selectedTeam |> SelectedTeam.getId))
  | _ =>
    failwith(
      "Turaku.currentTeam was called when on a page other than dashboard!",
    )
  };