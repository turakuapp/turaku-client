let str = ReasonReact.string;

type selection =
  | TeamMemberSelected(TeamMember.id)
  | ExistingInvitationSelected(InvitationToUser.id)
  | NewInvitationSelected
  | NothingSelected;

type state = {selection};

type action =
  | ShowInvitationForm
  | SelectTeamMember(TeamMember.id)
  | SelectInvitation(InvitationToUser.id);

let component = ReasonReact.reducerComponent("TeamMenu");

let showInvitationForm = (send, event) => {
  event |> DomUtils.preventMouseEventDefault;
  send(ShowInvitationForm);
};

let newInvitationButton = (state, send) =>
  switch (state.selection) {
  | NothingSelected
  | TeamMemberSelected(_)
  | ExistingInvitationSelected(_) =>
    <button className="mr-2 btn btn-blue" onClick={showInvitationForm(send)}>
      {"+" |> str}
    </button>
  | NewInvitationSelected => ReasonReact.null
  };

let containerClasses = (state, ~teamMember=?, ~invitation=?, ()) => {
  let classes = "cursor-pointer p-2 font-thin hover:bg-white flex justify-between items-center";

  let selected =
    switch (state.selection) {
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

let selectTeamMember = (teamMember, send, event) => {
  event |> DomUtils.preventMouseEventDefault;
  send(SelectTeamMember(teamMember |> TeamMember.id));
};

let selectInvitation = (invitation, send, event) => {
  event |> DomUtils.preventMouseEventDefault;
  send(SelectInvitation(invitation |> InvitationToUser.id));
};

let teamMemberOptions = (team, state, send) =>
  switch (team |> Team.teamMembers) {
  | [] => <div> {"Loading users..." |> str} </div>
  | teamMembers =>
    <div>
      {
        teamMembers
        |> List.map(teamMember =>
             <div
               onClick={selectTeamMember(teamMember, send)}
               className={containerClasses(state, ~teamMember, ())}
               key={teamMember |> TeamMember.id}>
               {teamMember |> TeamMember.name |> str}
             </div>
           )
        |> Array.of_list
        |> ReasonReact.array
      }
    </div>
  };

let invitedMembers = (team, state, send) =>
  team
  |> Team.invitations
  |> List.map(invitation =>
       <div
         onClick={selectInvitation(invitation, send)}
         className={containerClasses(state, ~invitation, ())}
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
  |> ReasonReact.array;

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

let inviteUser = (session, team, appSend, send, event) => {
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
         send(SelectInvitation(invitation |> InvitationToUser.id));
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

let hideInvitationForm = (team, send, event) => {
  event |> DomUtils.preventMouseEventDefault;

  switch (team |> defaultSelection) {
  | ExistingInvitationSelected(invitationId) =>
    send(SelectInvitation(invitationId))
  | TeamMemberSelected(teamMemberId) => send(SelectTeamMember(teamMemberId))
  | NewInvitationSelected
  | NothingSelected => ()
  };
};

let invitationForm = (session, team, appSend, send) =>
  <div className="mt-4 ml-2">
    <div className="flex">
      <div className="w-32 mr-2" />
      <div className="text-lg font-bold pl-2">
        {"Invite a team member" |> str}
      </div>
    </div>
    <form
      className="mt-4" onSubmit={inviteUser(session, team, appSend, send)}>
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
          onClick={hideInvitationForm(team, send)}>
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

let make = (~session, ~team, ~appSend, _children) => {
  ...component,
  didMount: _self => refreshUsers(session, team, appSend),
  initialState: () => {selection: team |> defaultSelection},
  reducer: (action, _state) =>
    switch (action) {
    | ShowInvitationForm =>
      ReasonReact.Update({selection: NewInvitationSelected})
    | SelectTeamMember(teamMemberId) =>
      ReasonReact.Update({selection: TeamMemberSelected(teamMemberId)})
    | SelectInvitation(invitationId) =>
      ReasonReact.Update({
        selection: ExistingInvitationSelected(invitationId),
      })
    },
  render: ({state, send}) =>
    <div className="flex">
      <div className="w-1/5 flex flex-col h-screen">
        <div className="mt-2 flex flex-no-shrink flex-row">
          <input
            type_="text"
            placeholder="Search"
            className="rounded flex-grow mx-2 pl-2 py-2"
          />
          {newInvitationButton(state, send)}
        </div>
        <div className="overflow-scroll mt-2">
          {invitedMembers(team, state, send)}
          {teamMemberOptions(team, state, send)}
        </div>
      </div>
      <div className="w-4/5 bg-white">
        {
          switch (state.selection) {
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
            invitationForm(session, team, appSend, send)
          | NothingSelected => editorPlaceholder
          }
        }
      </div>
    </div>,
};