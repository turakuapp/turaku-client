let str = React.string;

type selection =
  | TeamMemberSelected(TeamMember.id)
  | ExistingInvitationSelected(InvitationToUser.id)
  | NewInvitationSelected
  | NothingSelected;

let showInvitationForm = (setSelection, event) => {
  event |> DomUtils.preventMouseEventDefault;
  setSelection(_ => ShowInvitationForm);
};

let newInvitationButton = (selection, setSelection) =>
  switch (selection) {
  | NothingSelected
  | TeamMemberSelected(_)
  | ExistingInvitationSelected(_) =>
    <button
      className="mr-2 btn btn-blue"
      onClick={showInvitationForm(setSelection)}>
      {"+" |> str}
    </button>
  | NewInvitationSelected => React.null
  };

let containerClasses = (selection, ~teamMember=?, ~invitation=?, ()) => {
  let classes = "cursor-pointer p-2 font-thin hover:bg-white flex justify-between items-center";

  let selected =
    switch (selection) {
    | TeamMemberSelected(teamMemberId) =>
      switch (teamMember) {
      | Some(teamMember) => teamMember |> TeamMember.id == teamMemberId
      | None => false
      }
    | ExistingInvitationSelected(invitationId) =>
      switch (invitation) {
      | Some(invitation) => invitation |> InvitationToUser.id == invitationId
      | None => false
      }
    | NewInvitationSelected
    | NothingSelected => false
    };

  selected ? classes ++ " bg-white font-normal" : classes;
};

let selectTeamMember = (teamMember, setSelection, event) => {
  event |> DomUtils.preventMouseEventDefault;
  setSelection(_ => SelectTeamMember(teamMember |> TeamMember.id));
};

let selectInvitation = (invitation, setSelection, event) => {
  event |> DomUtils.preventMouseEventDefault;
  setSelection(_ => SelectInvitation(invitation |> InvitationToUser.id));
};

let teamMemberOptions = (team, selection, setSelection) =>
  switch (team |> Team.teamMembers) {
  | [] => <div> {"Loading users..." |> str} </div>
  | teamMembers =>
    <div>
      {
        teamMembers
        |> List.map(teamMember =>
             <div
               onClick={selectTeamMember(teamMember, setSelection)}
               className={containerClasses(selection, ~teamMember, ())}
               key={teamMember |> TeamMember.id}>
               {teamMember |> TeamMember.name |> str}
             </div>
           )
        |> Array.of_list
        |> React.array
      }
    </div>
  };

let invitedMembers = (team, selection, setSelection) =>
  team
  |> Team.invitations
  |> List.map(invitation =>
       <div
         onClick={selectInvitation(invitation, setSelection)}
         className={containerClasses(selection, ~invitation, ())}
         key={invitation |> InvitationToUser.id}>
         <em>
           {
             switch (invitation |> InvitationToUser.name) {
             | Some(name) => name |> str
             | None =>
               invitation |> InvitationToUser.email |> Email.toString |> str
             }
           }
         </em>
         {
           switch (invitation |> InvitationToUser.name) {
           | Some(_) => <i className="fas fa-envelope-open" />
           | None => <i className="fas fa-envelope" />
           }
         }
       </div>
     )
  |> Array.of_list
  |> React.array;

module CreateInvitation = [%graphql
  {|
  mutation($teamId: ID!, $email: String!) {
    createInvitation(teamId: $teamId, email: $email) {
      invitation {
        id
        invitedUser {
          name
        }
      }
      errors
    }
  }
  |}
];

let inviteUser = (session, team, appSend, setSelection, event) => {
  Js.log("Invite a new user!");
  event |> DomUtils.preventEventDefault;
  let email =
    DomUtils.getValueOfInputById("users__invite-form-email") |> Email.create;

  CreateInvitation.make(
    ~teamId=team |> Team.id,
    ~email=email |> Email.toString,
    (),
  )
  |> Api.sendAuthenticatedQuery(session)
  |> Js.Promise.then_(response => {
       switch (response##createInvitation##invitation) {
       | Some(invitation) =>
         let invitation =
           InvitationToUser.create(
             invitation##id,
             email,
             invitation##invitedUser##name,
           );
         appSend(Turaku.AddInvitationToUser(invitation));
         setSelection(_ =>
           SelectInvitation(invitation |> InvitationToUser.id)
         );
       | None =>
         Js.log2(
           "Failed to send invitations. Errors array: ",
           response##createInvitation##errors,
         )
       };

       /* Do something with this invitation. */
       Js.Promise.resolve();
     })
  |> ignore;
};

let defaultSelection = team =>
  switch (team |> Team.invitations) {
  | [invitation, ..._] =>
    ExistingInvitationSelected(invitation |> InvitationToUser.id)
  | [] =>
    switch (team |> Team.teamMembers) {
    | [teamMember, ..._] => TeamMemberSelected(teamMember |> TeamMember.id)
    | [] => NothingSelected
    }
  };

let hideInvitationForm = (team, setSelection, event) => {
  event |> DomUtils.preventMouseEventDefault;

  switch (team |> defaultSelection) {
  | ExistingInvitationSelected(invitationId) =>
    setSelection(_ => SelectInvitation(invitationId))
  | TeamMemberSelected(teamMemberId) =>
    setSelection(_ => SelectTeamMember(teamMemberId))
  | NewInvitationSelected
  | NothingSelected => ()
  };
};

let invitationForm = (session, team, appSend, setSelection) =>
  <div className="mt-4 ml-2">
    <div className="flex">
      <div className="w-32 mr-2" />
      <div className="text-lg font-bold pl-2">
        {"Invite a team member" |> str}
      </div>
    </div>
    <form
      className="mt-4"
      onSubmit={inviteUser(session, team, appSend, setSelection)}>
      <div className="flex mt-1">
        <div
          className="cursor-pointer w-32 font-thin hover:font-normal p-2 text-right mr-2">
          {"Email address" |> str}
        </div>
        <input
          className="w-1/2 p-2 rounded bg-grey-lighter"
          id="users__invite-form-email"
          type_="email"
          required=true
        />
      </div>
      <div className="flex mt-2">
        <div className="w-32 mr-2" />
        <button
          type_="submit"
          className="btn bg-green hover:bg-green-dark text-white">
          {"Send Invite" |> str}
        </button>
        <button
          className="ml-2 btn btn-blue"
          onClick={hideInvitationForm(team, setSelection)}>
          {"Cancel" |> str}
        </button>
      </div>
    </form>
  </div>;

module UsersQuery = [%graphql
  {|
  query($teamId: ID!) {
    team(id: $teamId) {
      teamMembers {
        id
        name
        email
      }
      invitations {
        id
        invitedUser {
          email
          name
        }
      }
    }
  }
  |}
];

let refreshUsers = (session, userTeam, appSend) =>
  UsersQuery.make(~teamId=userTeam |> Team.id, ())
  |> Api.sendAuthenticatedQuery(session)
  |> Js.Promise.then_(response =>
       switch (response##team) {
       | Some(team) =>
         let teamMembers =
           team##teamMembers
           |> Array.map(teamMember =>
                TeamMember.create(
                  teamMember##id,
                  teamMember##name,
                  teamMember##email |> Email.create,
                )
              )
           |> Array.to_list;

         let invitations =
           team##invitations
           |> Array.map(jsInvitation =>
                InvitationToUser.create(
                  jsInvitation##id,
                  jsInvitation##invitedUser##email |> Email.create,
                  jsInvitation##invitedUser##name,
                )
              )
           |> Array.to_list;

         appSend(
           Turaku.RefreshTeamMembers(
             userTeam |> Team.id,
             teamMembers,
             invitations,
           ),
         );

         Js.Promise.resolve();

       | None =>
         Webapi.Dom.window
         |> Webapi.Dom.Window.alert(
              "It looks like you have lost access to the current team. Reloading Turaku...",
            );
         Webapi.Dom.location |> Webapi.Dom.Location.reload;
         Js.Promise.resolve();
       }
     )
  |> ignore;

let editorPlaceholder =
  <div className="mt-2 ml-2">
    {"Select a team member, or invite someone." |> str}
  </div>;

[@react.component]
let make = (~session, ~team, ~appSend) => {
  let (selection, setSelection) =
    React.useState(() => team |> defaultSelection);

  <div className="flex">
    <div className="w-1/5 flex flex-col h-screen">
      <div className="mt-2 flex flex-no-shrink flex-row">
        <input
          type_="text"
          placeholder="Search"
          className="rounded flex-grow mx-2 pl-2 py-2"
        />
        {newInvitationButton(selection, setSelection)}
      </div>
      <div className="overflow-scroll mt-2">
        {invitedMembers(team, selection, setSelection)}
        {teamMemberOptions(team, selection, setSelection)}
      </div>
    </div>
    <div className="w-4/5 bg-white">
      {
        switch (selection) {
        | TeamMemberSelected(teamMemberId) =>
          switch (
            team
            |> Team.teamMembers
            |> ListUtils.find_opt(teamMember =>
                 teamMember |> TeamMember.id == teamMemberId
               )
          ) {
          | Some(teamMember) => <TeamMemberEditor teamMember appSend />
          | None => editorPlaceholder
          }

        | ExistingInvitationSelected(invitationId) =>
          switch (
            team
            |> Team.invitations
            |> ListUtils.find_opt(invitation =>
                 invitation |> InvitationToUser.id == invitationId
               )
          ) {
          | Some(invitation) =>
            <InvitationEditor session team invitation appSend />
          | None => editorPlaceholder
          }
        | NewInvitationSelected =>
          invitationForm(session, team, appSend, setSelection)
        | NothingSelected => editorPlaceholder
        }
      }
    </div>
  </div>;
};