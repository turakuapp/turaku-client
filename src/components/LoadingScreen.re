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
let make = (~appState, ~appSend, _children) => {
  ...component,
  didMount: _self => {
    let session = Session.attemptRestoration();
    switch (session) {
    | SignedOut =>
      /* Session couldn't be restored, so skip loading. */
      appSend(Turaku.SkipLoading)
    | SignedIn(credentials) =>
      /* Session info was loaded, so retrieve sign user back in. */
      RestoreSessionQuery.make()
      |> Api.sendQuery(session)
      |> Js.Promise.then_(response => {
           let teams =
             response##session##user##teams
             |> Array.map(team =>
                  Team.create(
                    team##id,
                    team##name,
                    EncryptedData.create(
                      team##encryptedPassword##iv
                      |> EncryptedData.InitializationVector.fromString,
                      team##encryptedPassword##ciphertext
                      |> EncryptedData.CipherText.fromString,
                    ),
                  )
                )
             |> Array.to_list;
           let invitations =
             response##session##user##incomingInvitations
             |> Array.map(i =>
                  Invitation.create(
                    i##id,
                    ~teamName=i##team##name,
                    ~userEmail=i##invitingUser##email,
                  )
                )
             |> Array.to_list;
           let encryptionHash = credentials |> Session.getEncryptionHash;
           let accessToken = credentials |> Session.getAccessToken;
           appSend(
             Turaku.SignIn(accessToken, teams, invitations, encryptionHash),
           );
           Js.Promise.resolve();
         })
      |> ignore
    };
    ReasonReact.NoUpdate;
  },
  render: _self => <div> ("Loading..." |> str) </div>,
};