let str = ReasonReact.string;
type bag = {
  userData: Turaku.userData,
  dashboardPageData: Turaku.dashboardPageData,
  teamMember: TeamMember.t,
};

let component = ReasonReact.statelessComponent("PermissionsEditor");

let make = (~bag: bag, ~appSend, _children) => {
  ...component,
  render: _self =>
    <span> ("Permissions for a team member go here." |> str) </span>,
};