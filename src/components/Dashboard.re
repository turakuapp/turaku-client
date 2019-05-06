let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("Dashboard");

let getMenu = (session, team, dashboardMenu, appSend) =>
  switch (dashboardMenu) {
  | Turaku.EntriesMenu => <Entries session team appSend />
  | TeamMenu => <TeamMenu session team appSend />
  };

let navigateToTeams = (appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  Turaku.DeselectTeam |> appSend;
};

let navigateToTeamMenu = (dashboardMenu, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  switch (dashboardMenu) {
  | Turaku.TeamMenu => ()
  | EntriesMenu => Turaku.SelectTeamMenu |> appSend
  };
};

let navigateToAllEntries = (dashboardMenu, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;

  switch (dashboardMenu) {
  | Turaku.EntriesMenu => ()
  | TeamMenu => Turaku.SelectTag(None) |> appSend
  };
};

let make = (~session, ~team, ~dashboardMenu, ~appSend, _children) => {
  ...component,
  render: _self =>
    /* <div className="flex justify-center h-screen">
       <div className="w-full md:w-1/2 self-auto md:self-center pt-4 md:pt-0"> */
    <div className="flex">
      <div
        className="w-1/6 h-screen bg-grey-light flex flex-col justify-between">
        <div className="mt-2">
          <div
            onClick={navigateToAllEntries(dashboardMenu, appSend)}
            className="cursor-pointer p-2 pl-4 hover:bg-grey-lighter text-sm font-thin tracking-wide">
            {"All entries" |> str}
          </div>
          <Tags team appSend />
          <EntryGroups />
        </div>
        <div className="mb-2">
          <div
            onClick={navigateToTeamMenu(dashboardMenu, appSend)}
            className="cursor-pointer p-2 pl-4 hover:bg-grey-lighter">
            {str("Members")}
          </div>
          <div
            className="cursor-pointer p-2 pl-4 hover:bg-grey-lighter"
            onClick={navigateToTeams(appSend)}>
            {str("Switch Team")}
          </div>
          <SignOutButton session appSend />
        </div>
      </div>
      <div className="w-5/6 bg-grey-lighter">
        {getMenu(session, team, dashboardMenu, appSend)}
        <span />
      </div>
    </div>,
};