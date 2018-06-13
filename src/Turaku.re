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

type userData = {
  session: Session.t,
  invitations: list(Invitation.t),
  teams: list(Team.t),
};

type user =
  | SignedOutUser(signedOutPage)
  | SignedInUser(signedInPage, userData);

type action =
  | SignUp
  | SignIn(Session.t, list(Team.t), list(Invitation.t))
  | RefreshEntries(Team.id, list(Entry.t), userData)
  | Navigate(user)
  | SkipLoading
  | CreateTeam(Team.t, userData)
  | SelectTeam(Team.id, userData)
  | SignOut(Session.t);

type state = {user};

let initialState = {user: SignedOutUser(LoadingPage)};

let reducer = (action, state) =>
  switch (action) {
  | SignIn(session, teams, invitations) =>
    ReasonReact.Update({
      user: SignedInUser(TeamSelectionPage, {session, invitations, teams}),
    })
  | SignUp =>
    ReasonReact.Update({
      user: SignedOutUser(SignInPage({justSignedUp: true})),
    })
  | Navigate(user) => ReasonReact.Update({user: user})
  | SkipLoading =>
    ReasonReact.Update({
      user: SignedOutUser(SignInPage({justSignedUp: false})),
    })
  | SelectTeam(teamId, userData) =>
    ReasonReact.Update({
      user:
        SignedInUser(
          DashboardPage({
            teamId,
            dashboardMenu: EntriesMenu({entryId: None}),
          }),
          userData,
        ),
    })
  | CreateTeam(team, userData) =>
    let updatedUserData = {...userData, teams: [team, ...userData.teams]};
    ReasonReact.Update({
      user:
        SignedInUser(
          DashboardPage({
            teamId: team |> Team.getId,
            dashboardMenu: EntriesMenu({entryId: None}),
          }),
          updatedUserData,
        ),
    });
  | SignOut(session) =>
    session |> Session.signOut;
    ReasonReact.Update({
      user: SignedOutUser(SignInPage({justSignedUp: false})),
    });
  | RefreshEntries(teamId, listOfEntries, userData) =>
    let entryId =
      switch (listOfEntries) {
      | [entry, ..._] => Some(entry |> Entry.getId)
      | [] => None
      };
    let updatedTeams =
      userData.teams
      |> List.map(team =>
           if (team |> Team.getId == teamId) {
             team |> Team.addEntries(listOfEntries);
           } else {
             team;
           }
         );
    ReasonReact.Update({
      user:
        SignedInUser(
          DashboardPage({
            teamId,
            dashboardMenu: EntriesMenu({entryId: entryId}),
          }),
          {...userData, teams: updatedTeams},
        ),
    });
  };

let currentTeam = (userData, dashboardPageData) =>
  userData.teams
  |> List.find(team => team |> Team.getId == dashboardPageData.teamId);

let currentEntry = (userData, dashboardPageData, entryId) =>
  currentTeam(userData, dashboardPageData)
  |> Team.getEntries
  |> List.find(entry => entry |> Entry.getId == entryId);