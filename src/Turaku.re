type signInPageData = {justSignedUp: bool};

type teamMenuSelection =
  | InvitationSelected
  | TeamMemberSelected;

type dashboardMenu =
  | EntriesMenu
  | TeamMenu(teamMenuSelection);

type signedOutPage =
  | LoadingPage
  | SignUpPage
  | SignInPage(signInPageData);

type userData = {
  session: Session.t,
  invitations: list(InvitationFromTeam.t),
  teams: SelectableList.t(Team.t),
  dashboardMenu,
};

type user =
  | SignedOutUser(signedOutPage)
  | SignedInUser(userData);

type action =
  | SignUp
  | SignIn(Session.t, list(Team.t), list(InvitationFromTeam.t))
  | RefreshEntries(Team.t, list(Entry.t), userData)
  | RefreshTeamMembers(
      Team.t,
      list(TeamMember.t),
      list(InvitationToUser.t),
      userData,
    )
  | Navigate(user)
  | SkipLoading
  | AddTeam(Team.t, userData)
  | SelectTeam(Team.t, userData)
  | SelectEntry(Team.t, Entry.t, userData)
  | SignOut(Session.t)
  | AddInvitationToUser(Team.t, InvitationToUser.t, userData)
  | RemoveInvitationToUser(Team.t, InvitationToUser.t, userData)
  | RemoveInvitationFromTeam(InvitationFromTeam.t, userData);

type state = {user};

let initialState = {user: SignedOutUser(LoadingPage)};

let reducer = (action, _state) =>
  switch (action) {
  | SignIn(session, teams, invitations) =>
    ReasonReact.Update({
      user:
        SignedInUser({
          dashboardMenu: EntriesMenu,
          session,
          invitations,
          teams: teams |> SelectableList.create,
        }),
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
  | SelectTeam(team, userData) =>
    ReasonReact.Update({
      user:
        SignedInUser({
          ...userData,
          teams: userData.teams |> SelectableList.select(team),
        }),
    })
  | SelectEntry(team, entry, userData) =>
    let updatedEntries = team |> Team.entries |> SelectableList.select(entry);
    let updatedTeam = team |> Team.replaceEntries(updatedEntries);
    ReasonReact.Update({
      user:
        SignedInUser({
          ...userData,
          teams: userData.teams |> SelectableList.replace(team, updatedTeam),
        }),
    });
  | AddTeam(team, userData) =>
    ReasonReact.Update({
      user:
        SignedInUser({
          ...userData,
          teams: userData.teams |> SelectableList.add(team),
        }),
    })
  | SignOut(session) =>
    session |> Session.signOut;
    ReasonReact.Update({
      user: SignedOutUser(SignInPage({justSignedUp: false})),
    });
  | RefreshEntries(team, entries, userData) =>
    let updatedTeam =
      team |> Team.replaceEntries(entries |> SelectableList.create);

    ReasonReact.Update({
      user:
        SignedInUser({
          ...userData,
          teams: userData.teams |> SelectableList.replace(team, updatedTeam),
        }),
    });
  | RefreshTeamMembers(team, teamMembers, invitations, userData) =>
    let updatedTeam =
      team
      |> Team.replaceTeamMembers(teamMembers |> SelectableList.create)
      |> Team.replaceInvitations(invitations |> SelectableList.create);

    let teamMenuSelection =
      switch (invitations) {
      | [_, ..._] => InvitationSelected
      | [] => TeamMemberSelected
      };
    ReasonReact.Update({
      user:
        SignedInUser({
          ...userData,
          teams: userData.teams |> SelectableList.replace(team, updatedTeam),
          dashboardMenu: TeamMenu(teamMenuSelection),
        }),
    });
  | AddInvitationToUser(team, invitation, userData) =>
    let updatedTeam = team |> Team.addInvitation(invitation);
    ReasonReact.Update({
      user:
        SignedInUser({
          ...userData,
          teams: userData.teams |> SelectableList.replace(team, updatedTeam),
          dashboardMenu: TeamMenu(InvitationSelected),
        }),
    });
  | RemoveInvitationToUser(team, invitation, userData) =>
    let updatedTeam = team |> Team.removeInvitation(invitation);
    ReasonReact.Update({
      user:
        SignedInUser({
          ...userData,
          teams: userData.teams |> SelectableList.replace(team, updatedTeam),
        }),
    });
  | RemoveInvitationFromTeam(invitation, userData) =>
    ReasonReact.Update({
      user:
        SignedInUser({
          ...userData,
          invitations:
            userData.invitations |> List.filter(i => i != invitation),
        }),
    })
  };

let selectEntry = (entry, team, userData) => {
  let updatedEntries = team |> Team.entries |> SelectableList.select(entry);
  let updatedTeam = team |> Team.replaceEntries(updatedEntries);
  let updatedTeams =
    userData.teams |> SelectableList.replace(team, updatedTeam);
  Navigate(SignedInUser({...userData, teams: updatedTeams}));
};

let selectTeamMember = (teamMember, team, userData) => {
  let updatedTeamMembers =
    team |> Team.teamMembers |> SelectableList.select(teamMember);
  let updatedTeam = team |> Team.replaceTeamMembers(updatedTeamMembers);
  let updatedTeams =
    userData.teams |> SelectableList.replace(team, updatedTeam);
  Navigate(
    SignedInUser({
      ...userData,
      teams: updatedTeams,
      dashboardMenu: TeamMenu(TeamMemberSelected),
    }),
  );
};

let selectInvitation = (invitation, team, userData) => {
  let updatedInvitations =
    team |> Team.invitations |> SelectableList.select(invitation);
  let updatedTeam = team |> Team.replaceInvitations(updatedInvitations);
  let updatedTeams =
    userData.teams |> SelectableList.replace(team, updatedTeam);
  Navigate(
    SignedInUser({
      ...userData,
      teams: updatedTeams,
      dashboardMenu: TeamMenu(InvitationSelected),
    }),
  );
};

let deselectTeam = userData => {
  let updatedTeams = userData.teams |> SelectableList.deselect;
  Navigate(SignedInUser({...userData, teams: updatedTeams}));
};

let selectTeamMenu = userData =>
  Navigate(
    SignedInUser({...userData, dashboardMenu: TeamMenu(TeamMemberSelected)}),
  );
/*
 let currentTeam = (userData, dashboardPageData) =>
   userData.teams
   |> List.find(team => team |> Team.id == dashboardPageData.teamId);

 let currentEntry = (userData, dashboardPageData, entryId) =>
   currentTeam(userData, dashboardPageData)
   |> Team.entries
   |> List.find(entry => entry |> Entry.id == entryId);

 let someEntry = (userData, dashboardPageData) => {
   let entries = currentTeam(userData, dashboardPageData) |> Team.entries;
   switch (entries) {
   | [entry, ..._] => Some(entry |> Entry.id)
   | [] => None
   };
 }; */