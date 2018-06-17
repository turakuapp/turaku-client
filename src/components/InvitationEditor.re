let str = ReasonReact.string;

type bag = {
  userData: Turaku.userData,
  dashboardPageData: Turaku.dashboardPageData,
  teamMenuData: Turaku.teamMenuData,
  invitation: InvitationToUser.t,
};

let component = ReasonReact.statelessComponent("InvitationEditor");

let make = (~bag: bag, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div>
      (
        "InvitationEditor for "
        ++ (bag.invitation |> InvitationToUser.email |> Email.toString)
        ++ " renders here."
        |> str
      )
    </div>,
};