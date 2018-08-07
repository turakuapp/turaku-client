[%bs.raw {|require("./teamMenu.css")|}];

let str = ReasonReact.string;

type selection =
  | TeamMemberSelected(TeamMember.id)
  | ExistingInvitationSelected(InvitationToUser.id)
  | NewInvitationSelected;

type state = {selection};

type action =
  | ShowInvitationForm
  | SelectTeamMember(TeamMember.id)
  | SelectInvitation(InvitationToUser.id);

type ctx = {
  userData: Turaku.userData,
  team: Team.t,
};

let component = ReasonReact.reducerComponent("TeamMenu");

let showInvitationForm = (send, event) => {
  event |> DomUtils.preventMouseEventDefault;
  send(ShowInvitationForm);
};

let newInvitationButton = (state, send) =>
  switch (state.selection) {
  | TeamMemberSelected(_)
  | ExistingInvitationSelected(_) =>
    <button className="mr-2 btn btn-blue" onClick=(showInvitationForm(send))>
      ("+" |> str)
    </button>
  | NewInvitationSelected => ReasonReact.null
  };

let containerClasses = ctx => {
  let classes = "mt-2 p-2";
  /* if (ctx |> isCurrentChoice) {
       classes ++ " entry-choice--chosen";
     } else { */
  classes;
  /* }; */
};

let selectTeamMember = (ctx, teamMember, send, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  send(SelectTeamMember(teamMember |> TeamMember.id));
};

let selectInvitation = (ctx, invitation, send, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  send(SelectInvitation(invitation |> InvitationToUser.id));
};

let teamMemberOptions = (ctx, send, appSend) =>
  switch (ctx.team |> Team.teamMembers) {
  | [] => <div> ("Loading users..." |> str) </div>
  | teamMembers =>
    <div>
      (
        teamMembers
        |> List.map(teamMember =>
             <div
               onClick=(selectTeamMember(ctx, teamMember, send, appSend))
               className=(containerClasses(ctx))
               key=(teamMember |> TeamMember.id)>
               (teamMember |> TeamMember.name |> str)
               (" - " |> str)
               <code>
                 (teamMember |> TeamMember.email |> Email.toString |> str)
               </code>
             </div>
           )
        |> Array.of_list
        |> ReasonReact.array
      )
    </div>
  };

let invitedMembers = (ctx, send, appSend) =>
  ctx.team
  |> Team.invitations
  |> List.map(invitation =>
       <div
         onClick=(selectInvitation(ctx, invitation, send, appSend))
         className=(containerClasses(ctx))
         key=(invitation |> InvitationToUser.id)>
         (invitation |> InvitationToUser.email |> Email.toString |> str)
         (" - " |> str)
         <code>
           (
             switch (invitation |> InvitationToUser.name) {
             | Some(name) => name |> str
             | None => <em> ("Unregistered" |> str) </em>
             }
           )
         </code>
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

let inviteUser = (ctx, appSend, send, event) => {
  Js.log("Invite a new user!");
  event |> DomUtils.preventEventDefault;
  let email =
    DomUtils.getValueOfInputById("users__invite-form-email") |> Email.create;

  CreateInvitation.make(
    ~teamId=ctx.team |> Team.id,
    ~email=email |> Email.toString,
    (),
  )
  |> Api.sendAuthenticatedQuery(ctx.userData.session)
  |> Js.Promise.then_(response => {
       switch (response##createInvitation##invitation) {
       | Some(invitation) =>
         let invitation =
           InvitationToUser.create(
             invitation##id,
             email,
             invitation##invitedUser##name,
           );
         appSend(
           Turaku.AddInvitationToUser(ctx.team, invitation, ctx.userData),
         );
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

let defaultSelection = ctx =>
  switch (ctx.team |> Team.invitations) {
  | [invitation, ..._] =>
    ExistingInvitationSelected(invitation |> InvitationToUser.id)
  | [] =>
    switch (ctx.team |> Team.teamMembers) {
    | [teamMember, ..._] => TeamMemberSelected(teamMember |> TeamMember.id)
    | [] => NewInvitationSelected
    }
  };

let hideInvitationForm = (ctx, send, event) => {
  event |> DomUtils.preventMouseEventDefault;

  switch (ctx |> defaultSelection) {
  | ExistingInvitationSelected(invitationId) =>
    send(SelectInvitation(invitationId))
  | TeamMemberSelected(teamMemberId) => send(SelectTeamMember(teamMemberId))
  | NewInvitationSelected => ()
  };
};

let invitationForm = (ctx, appSend, state, send) =>
  <form onSubmit=(inviteUser(ctx, appSend, send)) className="p-2">
    <div className="form-group">
      <label htmlFor="users__invite-form-email">
        ("Email Address" |> str)
      </label>
      <input
        className="form-control"
        id="users__invite-form-email"
        placeholder="Team member's email address"
        _type="email"
        required=true
      />
    </div>
    <button _type="submit" className="btn btn-primary">
      ("Send Invite" |> str)
    </button>
    <button
      className="btn btn-secondary ml-2"
      onClick=(hideInvitationForm(ctx, send))>
      ("Cancel" |> str)
    </button>
  </form>;

module UsersQuery = [%graphql
  {|
  query($teamId: ID!) {
    team(id: $teamId) {
      users {
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

let refreshUsers = (ctx, appSend) =>
  UsersQuery.make(~teamId=ctx.team |> Team.id, ())
  |> Api.sendAuthenticatedQuery(ctx.userData.session)
  |> Js.Promise.then_(response => {
       let teamMembers =
         response##team##users
         |> Array.map(jsUser =>
              TeamMember.create(
                jsUser##id,
                jsUser##name,
                jsUser##email |> Email.create,
              )
            )
         |> Array.to_list;
       let invitations =
         response##team##invitations
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
           ctx.team |> Team.id,
           teamMembers,
           invitations,
         ),
       );
       Js.Promise.resolve();
     })
  |> ignore;

let editorPlaceholder =
  <span> ("Select a team member, or invite someone." |> str) </span>;

let make = (~ctx, ~appSend, _children) => {
  ...component,
  didMount: _self => refreshUsers(ctx, appSend),
  initialState: () => {selection: ctx |> defaultSelection},
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
            _type="text"
            placeholder="Search"
            className="rounded flex-grow mx-2 pl-2 py-2"
          />
          (newInvitationButton(state, send))
        </div>
        <div className="overflow-scroll">
          (invitedMembers(ctx, send, appSend))
          (teamMemberOptions(ctx, send, appSend))
        </div>
      </div>
      <div className="w-4/5 bg-white">
        (
          switch (state.selection) {
          | TeamMemberSelected(teamMemberId) =>
            switch (
              ctx.team
              |> Team.teamMembers
              |> ListUtils.find_opt(teamMember =>
                   teamMember |> TeamMember.id == teamMemberId
                 )
            ) {
            | Some(teamMember) =>
              <TeamMemberEditor
                ctx={userData: ctx.userData, teamMember}
                appSend
              />
            | None => editorPlaceholder
            }

          | ExistingInvitationSelected(invitationId) =>
            switch (
              ctx.team
              |> Team.invitations
              |> ListUtils.find_opt(invitation =>
                   invitation |> InvitationToUser.id == invitationId
                 )
            ) {
            | Some(invitation) =>
              <InvitationEditor
                ctx={userData: ctx.userData, team: ctx.team, invitation}
                appSend
              />
            | None => editorPlaceholder
            }
          | NewInvitationSelected => invitationForm(ctx, appSend, state, send)
          }
        )
      </div>
    </div>,
};