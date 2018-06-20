let str = ReasonReact.string;
type ctx = {
  userData: Turaku.userData,
  teamMember: TeamMember.t,
};

let component = ReasonReact.statelessComponent("TeamMemberEditor");

let make = (~ctx: ctx, ~appSend, _children) => {
  ...component,
  render: _self =>
    <span>
      (
        "Editor for team member "
        ++ (ctx.teamMember |> TeamMember.email |> Email.toString)
        ++ " goes here."
        |> str
      )
    </span>,
};