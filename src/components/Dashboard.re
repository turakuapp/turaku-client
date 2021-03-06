let str = React.string;

let getMenu = (session, team) =>
  /* switch (dashboardMenu) {
     | Turaku.EntriesMenu => <Entries session team appSend />
     | TeamMenu => <TeamMenu session team appSend />
     }; */
  React.null;

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

[@react.component]
let make = (~log, ~session, ~signOut, ~teamId) => {
  /* Find team from teamId. Load data if needed. */
  let team = ();

  <div className="flex">
    <div
      className="w-1/6 h-screen bg-grey-light flex flex-col justify-between">
      <div className="mt-2">
        <div
          onClick={_ => ()}
          className="cursor-pointer p-2 pl-4 hover:bg-grey-lighter text-sm font-thin tracking-wide">
          {"All entries" |> str}
        </div>
        /* <Tags team /> */
        <EntryGroups />
      </div>
      <div className="mb-2">
        <div
          onClick={_ => ()}
          className="cursor-pointer p-2 pl-4 hover:bg-grey-lighter">
          {str("Members")}
        </div>
        <div
          className="cursor-pointer p-2 pl-4 hover:bg-grey-lighter"
          onClick={_ => ()}>
          {str("Switch Team")}
        </div>
        <SignOutButton session />
      </div>
    </div>
    <div className="w-5/6 bg-grey-lighter">
      /* {getMenu(session, team)} */
       <span /> </div>
  </div>;
};
