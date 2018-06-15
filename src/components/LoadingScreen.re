let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("LoadingScreen");

module RestoreSessionQuery = [%graphql
  {|
  query {
    session {
      user {
        encryptionSalt
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
          team {
            name
          }
          invitingUser {
            email
          }
        }
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
      |> Js.Promise.then_(response =>
           response##session##user##teams
           |> Team.decryptTeams(
                response,
                session |> Session.getCryptographicKey,
              )
         )
      |> Js.Promise.then_(responseAndTeams => {
           let (response, teams) = responseAndTeams;
           let invitations =
             response##session##user##incomingInvitations
             |> Array.map(i =>
                  InvitationFromTeam.create(
                    i##id,
                    ~teamName=i##team##name,
                    ~invitingUserEmail=i##invitingUser##email |> Email.create,
                  )
                )
             |> Array.to_list;
           appSend(Turaku.SignIn(session, teams, invitations));
           Js.Promise.resolve();
         })
      |> ignore
    };
  },
  render: _self => <div> ("Loading..." |> str) </div>,
};