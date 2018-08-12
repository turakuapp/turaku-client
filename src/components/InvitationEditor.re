let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  invitation: InvitationToUser.t,
  team: Team.t,
};

let component = ReasonReact.statelessComponent("InvitationEditor");

let deleteInvitation = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;

  let invitationId = ctx.invitation |> InvitationToUser.id;
  Js.log("Deleting invitation with ID: " ++ invitationId);

  ctx.invitation
  |> InvitationToUser.id
  |> Invitation.delete(ctx.userData.session)
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
    <div className="px-2">
      <div className="flex mt-2">
        <div className="w-32 mr-2" />
        <div className="text-lg font-bold p-2">
          ("Invitation to " |> str)
          <span className="text-blue-darker">
            (ctx.invitation |> InvitationToUser.email |> Email.toString |> str)
          </span>
        </div>
      </div>
      <div className="flex mt-2">
        <div
          className="cursor-pointer w-32 font-thin text-sm hover:font-normal p-2 text-right mr-2">
          ("Team Password" |> str)
        </div>
        <input
          className="w-1/2 p-2 rounded bg-white hover:bg-grey-lighter focus:bg-grey-lighter"
          id="entry-field__input-0"
          type_="text"
          value=(ctx.team |> Team.password |> TeamPassword.toString)
          readOnly=true
        />
      </div>
      <div className="flex mt-1">
        <div className="w-32 mr-2" />
        <small className="text-grey-dark pl-2">
          (
            "This new user will need the team password to join your team."
            |> str
          )
        </small>
      </div>
      <div className="flex mt-4">
        <div className="w-32 mr-2" />
        <button
          className="btn bg-red hover:bg-red-dark text-white"
          onClick=(deleteInvitation(ctx, appSend))>
          ("Delete Invitation" |> str)
        </button>
      </div>
    </div>,
};
