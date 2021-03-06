type signInPageData = {justSignedUp: bool};

type dashboardMenu =
  | EntriesMenu
  | TeamMenu;

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
  | RefreshEntries(Team.id, list(Entry.t), list(Tag.t))
  | RefreshTeamMembers(
      Team.id,
      list(TeamMember.t),
      list(InvitationToUser.t),
    )
  | SkipLoading
  | AddTeam(Team.t)
  | AcceptInvitation(Team.t, InvitationFromTeam.t)
  | SelectTeam(Team.t)
  | DeselectTeam
  | SelectEntry(Entry.t)
  | SelectTeamMenu
  | SelectTag(option(Tag.t))
  | SignOut(Session.t)
  | AddInvitationToUser(InvitationToUser.t)
  | RemoveInvitationToUser(InvitationToUser.t)
  | RemoveInvitationFromTeam(InvitationFromTeam.t)
  | AddNewEntry
  | EditEntryTitle(string)
  | EditEntryField(Field.t, int)
  | ReplaceEntry(Team.id, Entry.t, Entry.t)
  | RemoveTag(Entry.t, Tag.id)
  | AddTagToEntry(Entry.t, Tag.t);

let initialState = SignedOutUser(LoadingPage);

let withUser = (f, state) =>
  switch (state) {
  | SignedInUser(userData) => f(userData)
  | SignedOutUser(_) => state
  };

let withSelectedTeam = (f, state) =>
  state
  |> withUser(userData =>
       switch (userData.teams |> SelectableList.selected) {
       | Some(team) => f(team, userData)
       | None => state
       }
     );

let withTeam = (teamId, f, state) =>
  state
  |> withUser(userData => {
       let team =
         userData.teams
         |> SelectableList.all
         |> Array.of_list
         |> Js.Array.find(team => team |> Team.id == teamId);

       switch (team) {
       | Some(team) => f(team, userData)
       | None => state
       };
     });

let withSelectedEntry = (f, state) =>
  state
  |> withSelectedTeam((team, userData) =>
       switch (team |> Team.entries |> SelectableList.selected) {
       | Some(entry) => f(entry, team, userData)
       | None => state
       }
     );

let reducer = (state, action) =>
  switch (action) {
  | SelectSignIn => SignedOutUser(SignInPage({justSignedUp: false}))
  | SelectSignUp => SignedOutUser(SignUpPage)
  | SignIn(session, teams, invitations) =>
    let teamList =
      teams
      |> (
        teams |> List.length > 1 ?
          SelectableList.create : SelectableList.createAndSelect
      );

    SignedInUser({
      dashboardMenu: EntriesMenu,
      session,
      invitations,
      teams: teamList,
    });
  | SignUp => SignedOutUser(SignInPage({justSignedUp: true}))
  | SkipLoading => SignedOutUser(SignInPage({justSignedUp: false}))
  | SelectTeam(team) =>
    state
    |> withUser(userData =>
         SignedInUser({
           ...userData,
           teams: userData.teams |> SelectableList.select(team),
         })
       )

  | SelectEntry(entry) =>
    state
    |> withSelectedTeam((team, userData) => {
         let updatedEntries =
           team |> Team.entries |> SelectableList.select(entry);
         let updatedTeam = team |> Team.replaceEntries(updatedEntries);
         let updatedTeams =
           userData.teams |> SelectableList.replace(team, updatedTeam);

         SignedInUser({...userData, teams: updatedTeams});
       })
  | AddTeam(team) =>
    state
    |> withUser(userData =>
         SignedInUser({
           ...userData,
           teams: userData.teams |> SelectableList.add(team),
         })
       )

  | AcceptInvitation(team, invitationFromTeam) =>
    state
    |> withUser(userData =>
         SignedInUser({
           ...userData,
           teams: userData.teams |> SelectableList.add(team),
           invitations:
             userData.invitations |> List.filter(i => i != invitationFromTeam),
         })
       )
  | SignOut(session) =>
    session |> Session.signOut;
    SignedOutUser(SignInPage({justSignedUp: false}));
  | RefreshEntries(teamId, entries, tags) =>
    state
    |> withTeam(
         teamId,
         (team, userData) => {
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

           SignedInUser({
             ...userData,
             teams:
               userData.teams |> SelectableList.replace(team, updatedTeam),
           });
         },
       )
  | RefreshTeamMembers(teamId, teamMembers, invitations) =>
    state
    |> withTeam(
         teamId,
         (team, userData) => {
           let updatedTeam =
             team
             |> Team.replaceTeamMembers(teamMembers)
             |> Team.replaceInvitations(invitations);

           SignedInUser({
             ...userData,
             teams:
               userData.teams |> SelectableList.replace(team, updatedTeam),
             dashboardMenu: TeamMenu,
           });
         },
       )

  | AddInvitationToUser(invitation) =>
    state
    |> withSelectedTeam((team, userData) => {
         let updatedTeam = team |> Team.addInvitation(invitation);
         SignedInUser({
           ...userData,
           teams: userData.teams |> SelectableList.replace(team, updatedTeam),
           dashboardMenu: TeamMenu,
         });
       })
  | RemoveInvitationToUser(invitation) =>
    state
    |> withSelectedTeam((team, userData) => {
         let updatedTeam = team |> Team.removeInvitation(invitation);
         SignedInUser({
           ...userData,
           teams: userData.teams |> SelectableList.replace(team, updatedTeam),
         });
       })
  | RemoveInvitationFromTeam(invitation) =>
    state
    |> withUser(userData =>
         SignedInUser({
           ...userData,
           invitations:
             userData.invitations |> List.filter(i => i != invitation),
         })
       )
  | EditEntryTitle(title) =>
    state
    |> withSelectedEntry((entry, team, userData) => {
         let updatedEntry = entry |> Entry.editTitle(title);
         let updatedEntries =
           team |> Team.entries |> SelectableList.replace(entry, updatedEntry);
         let updatedTeam = team |> Team.replaceEntries(updatedEntries);
         SignedInUser({
           ...userData,
           teams: userData.teams |> SelectableList.replace(team, updatedTeam),
         });
       })
  | EditEntryField(field, index) =>
    state
    |> withSelectedEntry((entry, team, userData) => {
         let updatedEntry = entry |> Entry.editField(field, index);
         let updatedEntries =
           team |> Team.entries |> SelectableList.replace(entry, updatedEntry);
         let updatedTeam = team |> Team.replaceEntries(updatedEntries);
         SignedInUser({
           ...userData,
           teams: userData.teams |> SelectableList.replace(team, updatedTeam),
         });
       })
  | AddNewEntry =>
    state
    |> withSelectedTeam((team, userData) => {
         let updatedEntries =
           team |> Team.entries |> SelectableList.add(Entry.newUnsaved());
         let updatedTeam = team |> Team.replaceEntries(updatedEntries);
         SignedInUser({
           ...userData,
           teams: userData.teams |> SelectableList.replace(team, updatedTeam),
         });
       })

  | SelectTeamMenu =>
    state
    |> withSelectedTeam((_team, userData) =>
         SignedInUser({...userData, dashboardMenu: TeamMenu})
       )
  | DeselectTeam =>
    state
    |> withSelectedTeam((_team, userData) => {
         let updatedTeams = userData.teams |> SelectableList.deselect;
         SignedInUser({...userData, teams: updatedTeams});
       })
  | SelectTag(tag) =>
    state
    |> withSelectedTeam((_team, userData) =>
         switch (tag) {
         | None => SignedInUser({...userData, dashboardMenu: EntriesMenu})
         | Some(_tag) =>
           failwith("Tag selection hasn't been implemented yet.")
         }
       )
  | ReplaceEntry(teamId, oldEntry, newEntry) =>
    state
    |> withTeam(
         teamId,
         (team, userData) => {
           let updatedEntries =
             team
             |> Team.entries
             |> SelectableList.replace(oldEntry, newEntry);
           let updatedTeam = team |> Team.replaceEntries(updatedEntries);

           SignedInUser({
             ...userData,
             teams:
               userData.teams |> SelectableList.replace(team, updatedTeam),
           });
         },
       )
  | RemoveTag(eventEntry, tagId) =>
    state
    |> withSelectedEntry((entry, team, userData) =>
         if (entry |> Entry.id == (eventEntry |> Entry.id)) {
           let updatedEntries =
             team
             |> Team.entries
             |> SelectableList.replace(
                  entry,
                  entry |> Entry.removeTag(tagId),
                );

           let updatedTeam = team |> Team.replaceEntries(updatedEntries);

           SignedInUser({
             ...userData,
             teams:
               userData.teams |> SelectableList.replace(team, updatedTeam),
           });
         } else {
           state;
         }
       )
  | AddTagToEntry(entry, tag) =>
    state
    |> withSelectedEntry((selectedEntry, team, userData) => {
         let tagId = tag |> Tag.id;

         let updatedTeam =
           if (selectedEntry |> Entry.id == (entry |> Entry.id)) {
             let entryWithNewTag = selectedEntry |> Entry.addTag(tagId);
             let updatedEntries =
               team
               |> Team.entries
               |> SelectableList.replace(selectedEntry, entryWithNewTag);
             team |> Team.replaceEntries(updatedEntries);
           } else {
             team;
           };

         let existingTag =
           updatedTeam
           |> Team.tags
           |> SelectableList.all
           |> ListUtils.find_opt(existingTag => existingTag |> Tag.id == tagId);

         /* If the tag isn't in the list of team tags, we need to add it. */
         let updatedTeam =
           switch (existingTag) {
           | None =>
             let updatedTags =
               updatedTeam |> Team.tags |> SelectableList.add(tag);
             updatedTeam |> Team.replaceTags(updatedTags);
           | Some(_) => updatedTeam
           };

         SignedInUser({
           ...userData,
           teams: userData.teams |> SelectableList.replace(team, updatedTeam),
         });
       })
  };