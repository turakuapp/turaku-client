[%bs.raw {|require("./teamMenu.css")|}];
[%bs.raw {|require("./entryChoice.css")|}];

let str = ReasonReact.string;

type state = {inviteFormVisible: bool};

type action =
  | ToggleInviteForm;

type ctx = {
  userData: Turaku.userData,
  teamMenuSelection: Turaku.teamMenuSelection,
  team: Team.t,
};

let component = ReasonReact.reducerComponent("TeamMenu");

let toggleInviteForm = (send, event) => {
  event |> DomUtils.preventMouseEventDefault;
  send(ToggleInviteForm);
};

let invitationToggle = (state, send) =>
  if (state.inviteFormVisible) {
    ReasonReact.null;
  } else {
    <button
      className="btn btn-primary btn-sm" onClick=(toggleInviteForm(send))>
      ("Invite" |> str)
    </button>;
  };

let containerClasses = ctx => {
  let classes = "mt-2 p-2 entry-choice";
  /* if (ctx |> isCurrentChoice) {
       classes ++ " entry-choice--chosen";
     } else { */
  classes;
  /* }; */
};

let selectTeamMember = (ctx, teamMember, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  Turaku.SelectTeamMember(teamMember) |> appSend;
};

let selectInvitation = (ctx, invitation, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  Turaku.SelectInvitation(invitation) |> appSend;
};

let teamMemberOptions = (ctx, appSend) =>
  switch (ctx.team |> Team.teamMembers |> SelectableList.all) {
  | [] => <div> ("Loading users..." |> str) </div>
  | teamMembers =>
    <div>
      (
        teamMembers
        |> List.map(teamMember =>
             <div
               onClick=(selectTeamMember(ctx, teamMember, appSend))
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

let invitedMembers = (ctx, appSend) =>
  ctx.team
  |> Team.invitations
  |> SelectableList.all
  |> List.map(invitation =>
       <div
         onClick=(selectInvitation(ctx, invitation, appSend))
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
         send(ToggleInviteForm);
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

let invitationForm = (ctx, appSend, state, send) =>
  if (state.inviteFormVisible) {
    <form
      onSubmit=(inviteUser(ctx, appSend, send))
      className="p-2 team-menu__invite-form">
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
        className="btn btn-secondary ml-2" onClick=(toggleInviteForm(send))>
        ("Cancel" |> str)
      </button>
    </form>;
  } else {
    ReasonReact.null;
  };

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
  initialState: () => {inviteFormVisible: false},
  reducer: (action, state) =>
    switch (action) {
    | ToggleInviteForm =>
      ReasonReact.Update({inviteFormVisible: ! state.inviteFormVisible})
    },
  render: ({state, send}) =>
    <div className="row">
      <div className="col-3">
        <div className="team-menu__members">
          <div className="pt-2">
            <input _type="text" placeholder="Search" className="mr-2" />
            (invitationToggle(state, send))
          </div>
          (invitationForm(ctx, appSend, state, send))
          (invitedMembers(ctx, appSend))
          (teamMemberOptions(ctx, appSend))
        </div>
      </div>
      <div className="col team-menu__editor-container">
        (
          switch (ctx.teamMenuSelection) {
          | TeamMemberSelected =>
            switch (ctx.team |> Team.teamMembers |> SelectableList.selected) {
            | Some(teamMember) =>
              <TeamMemberEditor
                ctx={userData: ctx.userData, teamMember}
                appSend
              />
            | None => editorPlaceholder
            }

          | InvitationSelected =>
            switch (ctx.team |> Team.invitations |> SelectableList.selected) {
            | Some(invitation) =>
              <InvitationEditor
                ctx={userData: ctx.userData, team: ctx.team, invitation}
                appSend
              />
            | None => editorPlaceholder
            }
          }
        )
      </div>
    </div>,
};