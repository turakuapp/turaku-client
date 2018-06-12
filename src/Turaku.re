type signInPageData = {justSignedUp: bool};

type entryMenuData = {entryId: option(Entry.id)};

type dashboardMenu =
  | EntriesMenu(entryMenuData)
  | UsersMenu;

type dashboardPageData = {
  teamId: Team.id,
  dashboardMenu,
};

type signedInPage =
  | TeamSelectionPage
  | DashboardPage(dashboardPageData);

type signedOutPage =
  | LoadingPage
  | SignUpPage
  | SignInPage(signInPageData);

type signedInData = {
  session: Session.t,
  invitations: list(Invitation.t),
  teams: list(Team.t),
};

type user =
  | SignedOut(signedOutPage)
  | SignedIn(signedInPage, signedInData);

type action =
  | SignUp
  | SignIn(Session.t, list(Team.t), list(Invitation.t))
  | RefreshEntries(Team.id, list(Entry.t), signedInData)
  | Navigate(user)
  | SkipLoading
  | CreateTeam(Team.t, signedInData)
  | SelectTeam(Team.id, signedInData)
  | SignOut(Session.t);

type state = {user};

let initialState = {user: SignedOut(LoadingPage)};

let reducer = (action, state) =>
  switch (action) {
  | SignIn(session, teams, invitations) =>
    ReasonReact.Update({
      user: SignedIn(TeamSelectionPage, {session, invitations, teams}),
    })
  | SignUp =>
    ReasonReact.Update({user: SignedOut(SignInPage({justSignedUp: true}))})
  | Navigate(user) => ReasonReact.Update({user: user})
  | SkipLoading =>
    ReasonReact.Update({user: SignedOut(SignInPage({justSignedUp: false}))})
  | SelectTeam(teamId, signedInData) =>
    ReasonReact.Update({
      user:
        SignedIn(
          DashboardPage({
            teamId,
            dashboardMenu: EntriesMenu({entryId: None}),
          }),
          signedInData,
        ),
    })
  | CreateTeam(team, signedInData) =>
    let updatedSignedInData = {
      ...signedInData,
      teams: [team, ...signedInData.teams],
    };
    ReasonReact.Update({
      user:
        SignedIn(
          DashboardPage({
            teamId: team |> Team.getId,
            dashboardMenu: EntriesMenu({entryId: None}),
          }),
          updatedSignedInData,
        ),
    });
  | SignOut(session) =>
    session |> Session.signOut;
    ReasonReact.Update({
      user: SignedOut(SignInPage({justSignedUp: false})),
    });
  | RefreshEntries(teamId, listOfEntries, signedInData) =>
    let entryId =
      switch (listOfEntries) {
      | [entry, ..._] => Some(entry |> Entry.getId)
      | [] => None
      };
    let updatedTeams =
      signedInData.teams
      |> List.map(team =>
           if (team |> Team.getId == teamId) {
             team |> Team.addEntries(listOfEntries);
           } else {
             team;
           }
         );
    ReasonReact.Update({
      user:
        SignedIn(
          DashboardPage({
            teamId,
            dashboardMenu: EntriesMenu({entryId: entryId}),
          }),
          {...signedInData, teams: updatedTeams},
        ),
    });
  };

let currentTeam = (signedInData, dashboardPageData) =>
  signedInData.teams
  |> List.find(team => team |> Team.getId == dashboardPageData.teamId);

let currentEntry = (signedInData, dashboardPageData, entryId) =>
  currentTeam(signedInData, dashboardPageData)
  |> Team.getEntries
  |> List.find(entry => entry |> Entry.getId == entryId);