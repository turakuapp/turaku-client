let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  team: Team.t,
};

let component = ReasonReact.statelessComponent("Dashboard");

let getMenu = (ctx, appSend) =>
  switch (ctx.userData.dashboardMenu) {
  | Turaku.EntriesMenu =>
    <Entries session={ctx.userData.session} team={ctx.team} appSend />
  | TeamMenu =>
    <TeamMenu ctx={userData: ctx.userData, team: ctx.team} appSend />
  };

let navigateToTeams = (appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  Turaku.DeselectTeam |> appSend;
};

let navigateToTeamMenu = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  switch (ctx.userData.dashboardMenu) {
  | TeamMenu => ()
  | EntriesMenu => Turaku.SelectTeamMenu |> appSend
  };
};

let navigateToAllEntries = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  /* let entryId = Turaku.someEntry(ctx.userData, ctx.dashboardPageData); */
  switch (ctx.userData.dashboardMenu) {
  | EntriesMenu => ()
  | TeamMenu => Turaku.SelectTag(None) |> appSend
  };
};

let make = (~ctx: ctx, ~appSend, _children) => {
  ...component,
  render: _self =>
    /* <div className="flex justify-center h-screen">
       <div className="w-full md:w-1/2 self-auto md:self-center pt-4 md:pt-0"> */
    <div className="flex">
      <div
        className="w-1/6 h-screen bg-grey-light flex flex-col justify-between">
        <div className="mt-2">
          <div
            onClick={navigateToAllEntries(ctx, appSend)}
            className="cursor-pointer p-2 pl-4 hover:bg-grey-lighter text-sm font-thin tracking-wide">
            {"All entries" |> str}
          </div>
          <Tags ctx={userData: ctx.userData, team: ctx.team} appSend />
          <EntryGroups />
        </div>
        <div className="mb-2">
          <div
            onClick={navigateToTeamMenu(ctx, appSend)}
            className="cursor-pointer p-2 pl-4 hover:bg-grey-lighter">
            {str("Members")}
          </div>
          <div
            className="cursor-pointer p-2 pl-4 hover:bg-grey-lighter"
            onClick={navigateToTeams(appSend)}>
            {str("Switch Team")}
          </div>
          <SignOutButton ctx={userData: ctx.userData} appSend />
        </div>
      </div>
      <div className="w-5/6 bg-grey-lighter">
        {getMenu(ctx, appSend)}
        <span />
      </div>
    </div>,
};