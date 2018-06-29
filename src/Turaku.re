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

type state =
  | SignedOutUser(signedOutPage)
  | SignedInUser(userData);

type action =
  | SelectSignIn
  | SelectSignUp
  | SignUp
  | SignIn(Session.t, list(Team.t), list(InvitationFromTeam.t))
  | RefreshEntries(Team.t, list(Entry.t), userData)
  | RefreshTeamMembers(
      Team.t,
      list(TeamMember.t),
      list(InvitationToUser.t),
      userData,
    )
  | SkipLoading
  | AddTeam(Team.t, userData)
  | AcceptInvitation(Team.t, InvitationFromTeam.t, userData)
  | SelectTeam(Team.t, userData)
  | DeselectTeam(userData)
  | SelectEntry(Team.t, Entry.t, userData)
  | SelectTeamMenu(userData)
  | SelectTag(option(Tag.t), userData)
  | SignOut(Session.t)
  | AddInvitationToUser(Team.t, InvitationToUser.t, userData)
  | SelectInvitation(Team.t, InvitationToUser.t, userData)
  | SelectTeamMember(Team.t, TeamMember.t, userData)
  | RemoveInvitationToUser(Team.t, InvitationToUser.t, userData)
  | RemoveInvitationFromTeam(InvitationFromTeam.t, userData)
  | AddNewEntry(Team.t, userData)
  | EditEntryTitle(Team.t, Entry.t, string, userData)
  | EditEntryField(Team.t, Entry.t, Field.t, int, userData)
  | ReplaceEntry(Team.t, Entry.t, Entry.t, userData);

let initialState = SignedOutUser(LoadingPage);

let reducer = (action, _state) =>
  switch (action) {
  | SelectSignIn =>
    ReasonReact.Update(SignedOutUser(SignInPage({justSignedUp: false})))
  | SelectSignUp => ReasonReact.Update(SignedOutUser(SignUpPage))
  | SignIn(session, teams, invitations) =>
    let teamList =
      teams
      |> (
        teams |> List.length > 1 ?
          SelectableList.create : SelectableList.createAndSelect
      );
    ReasonReact.Update(
      SignedInUser({
        dashboardMenu: EntriesMenu,
        session,
        invitations,
        teams: teamList,
      }),
    );
  | SignUp =>
    ReasonReact.Update(SignedOutUser(SignInPage({justSignedUp: true})))
  | SkipLoading =>
    ReasonReact.Update(SignedOutUser(SignInPage({justSignedUp: false})))
  | SelectTeam(team, userData) =>
    ReasonReact.Update(
      SignedInUser({
        ...userData,
        teams: userData.teams |> SelectableList.select(team),
      }),
    )
  | SelectEntry(team, entry, userData) =>
    let updatedEntries = team |> Team.entries |> SelectableList.select(entry);
    let updatedTeam = team |> Team.replaceEntries(updatedEntries);
    let updatedTeams =
      userData.teams |> SelectableList.replace(team, updatedTeam);
    ReasonReact.Update(SignedInUser({...userData, teams: updatedTeams}));
  | AddTeam(team, userData) =>
    ReasonReact.Update(
      SignedInUser({
        ...userData,
        teams: userData.teams |> SelectableList.add(team),
      }),
    )
  | AcceptInvitation(team, invitationFromTeam, userData) =>
    ReasonReact.Update(
      SignedInUser({
        ...userData,
        teams: userData.teams |> SelectableList.add(team),
        invitations:
          userData.invitations |> List.filter(i => i != invitationFromTeam),
      }),
    )
  | SignOut(session) =>
    session |> Session.signOut;
    ReasonReact.Update(SignedOutUser(SignInPage({justSignedUp: false})));
  | RefreshEntries(team, entries, userData) =>
    let updatedTeam =
      team |> Team.replaceEntries(entries |> SelectableList.create);

    ReasonReact.Update(
      SignedInUser({
        ...userData,
        teams: userData.teams |> SelectableList.replace(team, updatedTeam),
      }),
    );
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
    ReasonReact.Update(
      SignedInUser({
        ...userData,
        teams: userData.teams |> SelectableList.replace(team, updatedTeam),
        dashboardMenu: TeamMenu(teamMenuSelection),
      }),
    );
  | AddInvitationToUser(team, invitation, userData) =>
    let updatedTeam = team |> Team.addInvitation(invitation);
    ReasonReact.Update(
      SignedInUser({
        ...userData,
        teams: userData.teams |> SelectableList.replace(team, updatedTeam),
        dashboardMenu: TeamMenu(InvitationSelected),
      }),
    );
  | RemoveInvitationToUser(team, invitation, userData) =>
    let updatedTeam = team |> Team.removeInvitation(invitation);
    ReasonReact.Update(
      SignedInUser({
        ...userData,
        teams: userData.teams |> SelectableList.replace(team, updatedTeam),
      }),
    );
  | RemoveInvitationFromTeam(invitation, userData) =>
    ReasonReact.Update(
      SignedInUser({
        ...userData,
        invitations: userData.invitations |> List.filter(i => i != invitation),
      }),
    )
  | EditEntryTitle(team, entry, title, userData) =>
    let updatedEntry = entry |> Entry.editTitle(title);
    let updatedEntries =
      team |> Team.entries |> SelectableList.replace(entry, updatedEntry);
    let updatedTeam = team |> Team.replaceEntries(updatedEntries);
    ReasonReact.Update(
      SignedInUser({
        ...userData,
        teams: userData.teams |> SelectableList.replace(team, updatedTeam),
      }),
    );
  | EditEntryField(team, entry, field, index, userData) =>
    let updatedEntry = entry |> Entry.editField(field, index);
    let updatedEntries =
      team |> Team.entries |> SelectableList.replace(entry, updatedEntry);
    let updatedTeam = team |> Team.replaceEntries(updatedEntries);
    ReasonReact.Update(
      SignedInUser({
        ...userData,
        teams: userData.teams |> SelectableList.replace(team, updatedTeam),
      }),
    );
  | AddNewEntry(team, userData) =>
    let updatedEntries =
      team |> Team.entries |> SelectableList.add(Entry.newUnsaved());
    let updatedTeam = team |> Team.replaceEntries(updatedEntries);
    ReasonReact.Update(
      SignedInUser({
        ...userData,
        teams: userData.teams |> SelectableList.replace(team, updatedTeam),
      }),
    );
  | SelectTeamMenu(userData) =>
    ReasonReact.Update(
      SignedInUser({
        ...userData,
        dashboardMenu: TeamMenu(TeamMemberSelected),
      }),
    )
  | DeselectTeam(userData) =>
    let updatedTeams = userData.teams |> SelectableList.deselect;
    ReasonReact.Update(SignedInUser({...userData, teams: updatedTeams}));
  | SelectInvitation(team, invitation, userData) =>
    let updatedInvitations =
      team |> Team.invitations |> SelectableList.select(invitation);
    let updatedTeam = team |> Team.replaceInvitations(updatedInvitations);
    let updatedTeams =
      userData.teams |> SelectableList.replace(team, updatedTeam);
    ReasonReact.Update(
      SignedInUser({
        ...userData,
        teams: updatedTeams,
        dashboardMenu: TeamMenu(InvitationSelected),
      }),
    );
  | SelectTeamMember(team, teamMember, userData) =>
    let updatedTeamMembers =
      team |> Team.teamMembers |> SelectableList.select(teamMember);
    let updatedTeam = team |> Team.replaceTeamMembers(updatedTeamMembers);
    let updatedTeams =
      userData.teams |> SelectableList.replace(team, updatedTeam);
    ReasonReact.Update(
      SignedInUser({
        ...userData,
        teams: updatedTeams,
        dashboardMenu: TeamMenu(TeamMemberSelected),
      }),
    );
  | SelectTag(tag, userData) =>
    switch (tag) {
    | None =>
      ReasonReact.Update(
        SignedInUser({...userData, dashboardMenu: EntriesMenu}),
      )
    | Some(tag) =>
      Js.log("Show entries with this tag, probably?");
      failwith("Tag selection hasn't been implemented yet.");
    }
  | ReplaceEntry(team, oldEntry, newEntry, userData) =>
    let updatedEntries =
      team |> Team.entries |> SelectableList.replace(oldEntry, newEntry);
    let updatedTeam = team |> Team.replaceEntries(updatedEntries);
    ReasonReact.Update(
      SignedInUser({
        ...userData,
        teams: userData.teams |> SelectableList.replace(team, updatedTeam),
      }),
    );
  };