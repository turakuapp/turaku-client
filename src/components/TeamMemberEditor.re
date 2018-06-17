let str = ReasonReact.string;
type bag = {
  userData: Turaku.userData,
  dashboardPageData: Turaku.dashboardPageData,
  teamMenuData: Turaku.teamMenuData,
  teamMember: TeamMember.t,
};

let component = ReasonReact.statelessComponent("TeamMemberEditor");

let make = (~bag: bag, ~appSend, _children) => {
  ...component,
  render: _self =>
    <span>
      (
        "Editor for team member "
        ++ (bag.teamMember |> TeamMember.email |> Email.toString)
        ++ " goes here."
        |> str
      )
    </span>,
};