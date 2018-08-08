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
  | NothingSelected
  | TeamMemberSelected(_)
  | ExistingInvitationSelected(_) =>
    <button className="mr-2 btn btn-blue" onClick=(showInvitationForm(send))>
      ("+" |> str)
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

let selectTeamMember = (ctx, teamMember, send, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  send(SelectTeamMember(teamMember |> TeamMember.id));
};

let selectInvitation = (ctx, invitation, send, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  send(SelectInvitation(invitation |> InvitationToUser.id));
};

let teamMemberOptions = (ctx, state, send, appSend) =>
  switch (ctx.team |> Team.teamMembers) {
  | [] => <div> ("Loading users..." |> str) </div>
  | teamMembers =>
    <div>
      (
        teamMembers
        |> List.map(teamMember =>
             <div
               onClick=(selectTeamMember(ctx, teamMember, send, appSend))
               className=(containerClasses(state, ~teamMember, ()))
               key=(teamMember |> TeamMember.id)>
               (teamMember |> TeamMember.name |> str)
             </div>
           )
        |> Array.of_list
        |> ReasonReact.array
      )
    </div>
  };

let invitedMembers = (ctx, state, send, appSend) =>
  ctx.team
  |> Team.invitations
  |> List.map(invitation =>
       <div
         onClick=(selectInvitation(ctx, invitation, send, appSend))
         className=(containerClasses(state, ~invitation, ()))
         key=(invitation |> InvitationToUser.id)>
         <em>
           (
             switch (invitation |> InvitationToUser.name) {
             | Some(name) => name |> str
             | None =>
               invitation |> InvitationToUser.email |> Email.toString |> str
             }
           )
         </em>
         (
           switch (invitation |> InvitationToUser.name) {
           | Some(_) => <Icon kind=Icon.EnvelopeOpen size=Icon.Size.Md />
           | None => <Icon kind=Icon.Envelope size=Icon.Size.Md />
           }
         )
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
    | [] => NothingSelected
    }
  };

let hideInvitationForm = (ctx, send, event) => {
  event |> DomUtils.preventMouseEventDefault;

  switch (ctx |> defaultSelection) {
  | ExistingInvitationSelected(invitationId) =>
    send(SelectInvitation(invitationId))
  | TeamMemberSelected(teamMemberId) => send(SelectTeamMember(teamMemberId))
  | NewInvitationSelected
  | NothingSelected => ()
  };
};

let invitationForm = (ctx, appSend, state, send) =>
  <div className="mt-4 ml-2">
    <div className="flex">
      <div className="w-32 mr-2" />
      <div className="text-lg font-bold pl-2">
        ("Invite a team member" |> str)
      </div>
    </div>
    <form className="mt-4" onSubmit=(inviteUser(ctx, appSend, send))>
      <div className="flex mt-1">
        <div
          className="cursor-pointer w-32 font-thin hover:font-normal p-2 text-right mr-2">
          ("Email address" |> str)
        </div>
        <input
          className="w-1/2 p-2 rounded bg-grey-lighter"
          id="users__invite-form-email"
          _type="email"
          required=true
        />
      </div>
      <div className="flex mt-2">
        <div className="w-32 mr-2" />
        <button
          _type="submit"
          className="btn bg-green hover:bg-green-dark text-white">
          ("Send Invite" |> str)
        </button>
        <button
          className="ml-2 btn btn-blue"
          onClick=(hideInvitationForm(ctx, send))>
          ("Cancel" |> str)
        </button>
      </div>
    </form>
  </div>;

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
  <div className="mt-2 ml-2">
    ("Select a team member, or invite someone." |> str)
  </div>;

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
        <div className="overflow-scroll mt-2">
          (invitedMembers(ctx, state, send, appSend))
          (teamMemberOptions(ctx, state, send, appSend))
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
          | NothingSelected => editorPlaceholder
          }
        )
      </div>
    </div>,
};