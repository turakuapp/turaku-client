let str = ReasonReact.string;

type bag = {
  userData: Turaku.userData,
  dashboardPageData: Turaku.dashboardPageData,
  teamMenuData: Turaku.teamMenuData,
  invitation: InvitationToUser.t,
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

let deleteInvitation = (bag, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;

  let invitationId = bag.invitation |> InvitationToUser.id;
  Js.log("Deleting invitation with ID: " ++ invitationId);

  let teamId =
    Turaku.currentTeam(bag.userData, bag.dashboardPageData) |> Team.id;

  DeleteInvitationQuery.make(~invitationId, ())
  |> Api.sendAuthenticatedQuery(bag.userData.session)
  |> Js.Promise.then_(response => {
       switch (response##deleteInvitation##errors |> Array.to_list) {
       | [] =>
         appSend(
           Turaku.RemoveInvitationToUser(
             teamId,
             bag.invitation,
             bag.userData,
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

let make = (~bag: bag, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div className="m-3">
      <button
        className="btn btn-danger" onClick=(deleteInvitation(bag, appSend))>
        ("Delete Invitation" |> str)
      </button>
    </div>,
};