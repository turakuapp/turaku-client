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
  | RefreshEntries(list(Entry.t), list(Tag.t))
  | RefreshTeamMembers(
      Team.id,
      list(TeamMember.t),
      list(InvitationToUser.t),
    )
  | SkipLoading
  | AddTeam(Team.t)
  | AcceptInvitation(Team.t, InvitationFromTeam.t, userData)
  | SelectTeam(Team.t)
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
  | AddNewEntry
  | EditEntryTitle(Team.t, Entry.t, string, userData)
  | EditEntryField(Team.t, Entry.t, Field.t, int, userData)
  | ReplaceEntry(Team.id, Entry.t, Entry.t);

let initialState = SignedOutUser(LoadingPage);

let withSelectedTeam = (f, state) =>
  switch (state) {
  | SignedInUser(userData) =>
    switch (userData.teams |> SelectableList.selected) {
    | Some(team) => f(team, userData)
    | None => ReasonReact.NoUpdate
    }
  | SignedOutUser(_) => ReasonReact.NoUpdate
  };

let withUser = (f, state) =>
  switch (state) {
  | SignedInUser(userData) => f(userData)
  | SignedOutUser(_) => ReasonReact.NoUpdate
  };

let reducer = (action, state) =>
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
  | SelectTeam(team) =>
    state
    |> withUser(userData =>
         ReasonReact.Update(
           SignedInUser({
             ...userData,
             teams: userData.teams |> SelectableList.select(team),
           }),
         )
       )

  | SelectEntry(team, entry, userData) =>
    let updatedEntries = team |> Team.entries |> SelectableList.select(entry);
    let updatedTeam = team |> Team.replaceEntries(updatedEntries);
    let updatedTeams =
      userData.teams |> SelectableList.replace(team, updatedTeam);
    ReasonReact.Update(SignedInUser({...userData, teams: updatedTeams}));
  | AddTeam(team) =>
    state
    |> withUser(userData =>
         ReasonReact.Update(
           SignedInUser({
             ...userData,
             teams: userData.teams |> SelectableList.add(team),
           }),
         )
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
  | RefreshEntries(entries, tags) =>
    state
    |> withSelectedTeam((team, userData) => {
         let updatedEntries = entries |> SelectableList.create;

         let updatedEntries =
           switch (team |> Team.entries |> SelectableList.selected) {
           | Some(previousEntry) =>
             switch (
               entries
               |> Array.of_list
               |> Js.Array.find(loadedEntry =>
                    loadedEntry |> Entry.id == (previousEntry |> Entry.id)
                  )
             ) {
             | Some(loadedSelectedEntry) =>
               updatedEntries |> SelectableList.select(loadedSelectedEntry)
             | None => updatedEntries
             }
           | None => updatedEntries
           };

         let updatedTeam =
           team
           |> Team.replaceEntries(updatedEntries)
           |> Team.replaceTags(tags |> SelectableList.create);

         ReasonReact.Update(
           SignedInUser({
             ...userData,
             teams:
               userData.teams |> SelectableList.replace(team, updatedTeam),
           }),
         );
       })
  | RefreshTeamMembers(teamId, teamMembers, invitations) =>
    state
    |> withSelectedTeam((team, userData) =>
         if (team |> Team.id == teamId) {
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
               teams:
                 userData.teams |> SelectableList.replace(team, updatedTeam),
               dashboardMenu: TeamMenu(teamMenuSelection),
             }),
           );
         } else {
           ReasonReact.NoUpdate;
         }
       )

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
  | AddNewEntry =>
    state
    |> withSelectedTeam((team, userData) => {
         let updatedEntries =
           team |> Team.entries |> SelectableList.add(Entry.newUnsaved());
         let updatedTeam = team |> Team.replaceEntries(updatedEntries);
         ReasonReact.Update(
           SignedInUser({
             ...userData,
             teams:
               userData.teams |> SelectableList.replace(team, updatedTeam),
           }),
         );
       })

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
  | ReplaceEntry(teamId, oldEntry, newEntry) =>
    state
    |> withUser(userData => {
         let team =
           userData.teams
           |> SelectableList.all
           |> Array.of_list
           |> Js.Array.find(team => team |> Team.id == teamId);

         switch (team) {
         | Some(team) =>
           let updatedEntries =
             team
             |> Team.entries
             |> SelectableList.replace(oldEntry, newEntry);
           let updatedTeam = team |> Team.replaceEntries(updatedEntries);

           ReasonReact.Update(
             SignedInUser({
               ...userData,
               teams:
                 userData.teams |> SelectableList.replace(team, updatedTeam),
             }),
           );
         | None => ReasonReact.NoUpdate
         };
       })
  };