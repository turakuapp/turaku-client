let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  team: Team.t,
};

let component = ReasonReact.statelessComponent("Dashboard");

let getMenu = (ctx, appSend) =>
  switch (ctx.userData.dashboardMenu) {
  | Turaku.EntriesMenu =>
    <Entries ctx={userData: ctx.userData, team: ctx.team} appSend />
  | TeamMenu(teamMenuSelection) =>
    <TeamMenu
      ctx={userData: ctx.userData, team: ctx.team, teamMenuSelection}
      appSend
    />
  };

let navigateToTeams = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  Turaku.DeselectTeam |> appSend;
};

let navigateToTeamMenu = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  switch (ctx.userData.dashboardMenu) {
  | TeamMenu(_) => ()
  | EntriesMenu => Turaku.SelectTeamMenu |> appSend
  };
};

let make = (~ctx: ctx, ~appSend, _children) => {
  ...component,
  render: _self =>
    /* <div className="flex justify-center h-screen">
       <div className="w-full md:w-1/2 self-auto md:self-center pt-4 md:pt-0"> */
    <div className="flex">
      <div className="w-1/6 h-screen bg-grey flex flex-col justify-between">
        <Tags ctx={userData: ctx.userData, team: ctx.team} appSend />
        <div>
          <div
            onClick=(navigateToTeamMenu(ctx, appSend))
            className="cursor-pointer p-1 hover:bg-grey-light">
            (str("Members"))
          </div>
          <div
            className="cursor-pointer p-1 hover:bg-grey-light"
            onClick=(navigateToTeams(ctx, appSend))>
            (str("Switch Team"))
          </div>
          <SignOutButton ctx={userData: ctx.userData} appSend />
        </div>
      </div>
      <div className="w-5/6 bg-grey-light">
        (getMenu(ctx, appSend))
        <span />
      </div>
    </div>,
};