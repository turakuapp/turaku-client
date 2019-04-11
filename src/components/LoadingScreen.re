exception RestoreSessionFailure;

let handleRestoreSessionFailure = appSend =>
  [@bs.open]
  (
    fun
    | RestoreSessionFailure => {
        appSend(Turaku.SkipLoading);
        Js.Promise.resolve();
      }
  );

let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("LoadingScreen");

module RestoreSessionQuery = [%graphql
  {|
  query {
    teams {
      id
      name
      encryptedPassword {
        iv
        ciphertext
      }
    }
    incomingInvitations {
      id
      teamName
      invitingUser {
        email
      }
    }
  }
  |}
];

/* TODO: This component should load necessary data, and accept password to regenerate the encryptionSalt. */
let make = (~appSend, _children) => {
  ...component,
  didMount: _self => {
    let session = Session.attemptRestoration();
    switch (session) {
    | None =>
      /* Session couldn't be restored, so skip loading. */
      appSend(Turaku.SkipLoading)
    | Some(session) =>
      /* Session info was loaded, so retrieve sign user back in. */
      RestoreSessionQuery.make()
      |> Api.sendAuthenticatedQuery(session)
      |> Js.Promise.then_(response => {
           let teams =
             response##teams
             |> Team.decryptTeams(session |> Session.getCryptographicKey);

           let invitations =
             response##incomingInvitations
             |> Array.map(i =>
                  InvitationFromTeam.create(
                    i##id,
                    ~teamName=i##teamName,
                    ~invitingUserEmail=i##invitingUser##email |> Email.create,
                  )
                )
             |> Array.to_list
             |> Js.Promise.resolve;

           Js.Promise.all2((teams, invitations));
         })
      |> Js.Promise.then_(((teams, invitations)) => {
           appSend(Turaku.SignIn(session, teams, invitations));
           Js.Promise.resolve();
         })
      |> Js.Promise.catch(error =>
           switch (error |> handleRestoreSessionFailure(appSend)) {
           | Some(x) => x
           | None =>
             Js.log(error);
             appSend(Turaku.SkipLoading);
             Js.Promise.resolve();
           }
         )
      |> ignore
    };
  },
  render: _self => <div> {"Loading..." |> str} </div>,
};