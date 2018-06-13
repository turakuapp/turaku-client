type signInPageData = {justSignedUp: bool};

type entryMenuData = {entryId: option(Entry.id)};

type dashboardMenu =
  | EntriesMenu(entryMenuData)
  | TeamMenu;

type dashboardPageData = {
  teamId: Team.id,
  menu: dashboardMenu,
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
  page: signedInPage,
};

type user =
  | SignedOutUser(signedOutPage)
  | SignedInUser(userData);

type action =
  | SignUp
  | SignIn(Session.t, list(Team.t), list(Invitation.t))
  | RefreshEntries(Team.id, list(Entry.t), userData)
  | RefreshTeamMembers(Team.id, list(TeamMember.t), userData)
  | Navigate(user)
  | SkipLoading
  | CreateTeam(Team.t, userData)
  | SelectTeam(Team.id, userData)
  | SignOut(Session.t);

type state = {user};

let initialState = {user: SignedOutUser(LoadingPage)};

let reducer = (action, _state) =>
  switch (action) {
  | SignIn(session, teams, invitations) =>
    ReasonReact.Update({
      user:
        SignedInUser({page: TeamSelectionPage, session, invitations, teams}),
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
        SignedInUser({
          ...userData,
          page: DashboardPage({teamId, menu: EntriesMenu({entryId: None})}),
        }),
    })
  | CreateTeam(team, userData) =>
    ReasonReact.Update({
      user:
        SignedInUser({
          ...userData,
          page:
            DashboardPage({
              teamId: team |> Team.id,
              menu: EntriesMenu({entryId: None}),
            }),
          teams: [team, ...userData.teams],
        }),
    })
  | SignOut(session) =>
    session |> Session.signOut;
    ReasonReact.Update({
      user: SignedOutUser(SignInPage({justSignedUp: false})),
    });
  | RefreshEntries(teamId, entries, userData) =>
    let entryId =
      switch (entries) {
      | [entry, ..._] => Some(entry |> Entry.getId)
      | [] => None
      };
    let updatedTeams =
      userData.teams
      |> List.map(team =>
           if (team |> Team.id == teamId) {
             team |> Team.addEntries(entries);
           } else {
             team;
           }
         );
    ReasonReact.Update({
      user:
        SignedInUser({
          ...userData,
          teams: updatedTeams,
          page:
            DashboardPage({teamId, menu: EntriesMenu({entryId: entryId})}),
        }),
    });
  | RefreshTeamMembers(teamId, teamMembers, userData) =>
    let updatedTeams =
      userData.teams
      |> List.map(team =>
           if (team |> Team.id == teamId) {
             team |> Team.addTeamMembers(teamMembers);
           } else {
             team;
           }
         );
    ReasonReact.Update({
      user: SignedInUser({...userData, teams: updatedTeams}),
    });
  };

let currentTeam = (userData, dashboardPageData) =>
  userData.teams
  |> List.find(team => team |> Team.id == dashboardPageData.teamId);

let currentEntry = (userData, dashboardPageData, entryId) =>
  currentTeam(userData, dashboardPageData)
  |> Team.entries
  |> List.find(entry => entry |> Entry.getId == entryId);