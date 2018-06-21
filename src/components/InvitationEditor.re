let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  invitation: InvitationToUser.t,
  team: Team.t,
};

let component = ReasonReact.statelessComponent("InvitationEditor");

module DeleteInvitationQuery = [%graphql
  {|
  mutation($invitationId: ID!) {
    deleteInvitation(id: $invitationId) {
      errors
    }
  }
  |}
];

let deleteInvitation = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;

  let invitationId = ctx.invitation |> InvitationToUser.id;
  Js.log("Deleting invitation with ID: " ++ invitationId);

  DeleteInvitationQuery.make(~invitationId, ())
  |> Api.sendAuthenticatedQuery(ctx.userData.session)
  |> Js.Promise.then_(response => {
       switch (response##deleteInvitation##errors |> Array.to_list) {
       | [] =>
         appSend(
           Turaku.RemoveInvitationToUser(
             ctx.team,
             ctx.invitation,
             ctx.userData,
           ),
         )
       | errors =>
         Js.log2(
           "Failed to delete invitation! Errors: ",
           errors |> Array.of_list |> Js.Array.joinWith(", "),
         )
       };
       Js.Promise.resolve();
     })
  |> ignore;
};

let make = (~ctx: ctx, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div className="m-3">
      <button
        className="btn btn-danger" onClick=(deleteInvitation(ctx, appSend))>
        ("Delete Invitation" |> str)
      </button>
    </div>,
};