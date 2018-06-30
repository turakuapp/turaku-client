let str = ReasonReact.string;

type state = {teamPassword: string};

type action =
  | UpdateTeamPassword(string);

type ctx = {
  userData: Turaku.userData,
  invitation: InvitationFromTeam.t,
};

let component = ReasonReact.reducerComponent("IncomingInvitation");

module AcceptInvitationQuery = [%graphql
  {|
  mutation($id: ID!, $iv: String!, $ciphertext: String!) {
    acceptInvitation(id: $id, encryptedPassword: {iv: $iv, ciphertext: $ciphertext}) {
      invitation {
        team {
          id
        }
      }
      errors
    }
  }
  |}
];

let acceptInvitation = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;

  let invitationId = ctx.invitation |> InvitationFromTeam.id;
  Js.log("Accepting Invitation#" ++ invitationId);

  let stringPassword =
    DomUtils.getValueOfInputById("invitations__team-password");

  let encryptionKey = ctx.userData.session |> Session.getCryptographicKey;
  EncryptedData.encrypt(encryptionKey, stringPassword)
  |> Js.Promise.then_(encryptedData => {
       let iv =
         encryptedData |> EncryptedData.iv |> InitializationVector.toString;
       let ciphertext =
         encryptedData |> EncryptedData.ciphertext |> CipherText.toString;

       AcceptInvitationQuery.make(
         ~id=ctx.invitation |> InvitationFromTeam.id,
         ~iv,
         ~ciphertext,
         (),
       )
       |> Api.sendAuthenticatedQuery(ctx.userData.session);
     })
  |> Js.Promise.then_(response => {
       let invitation = response##acceptInvitation##invitation;
       switch (invitation) {
       | Some(i) =>
         let team =
           Team.create(
             i##team##id,
             ctx.invitation |> InvitationFromTeam.name,
             stringPassword |> TeamPassword.fromString,
           );
         appSend(
           Turaku.AcceptInvitation(team, ctx.invitation, ctx.userData),
         );
       | None => Js.log2("Errors: ", response##acceptInvitation##errors)
       };
       Js.Promise.resolve();
     })
  |> ignore;
};

let rejectInvitation = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;

  let invitationId = ctx.invitation |> InvitationFromTeam.id;
  Js.log("Rejecting Invitation#" ++ invitationId);

  invitationId
  |> Invitation.delete(ctx.userData.session)
  |> Js.Promise.then_(response => {
       switch (response##deleteInvitation##errors |> Array.to_list) {
       | [] =>
         appSend(
           Turaku.RemoveInvitationFromTeam(ctx.invitation, ctx.userData),
         )
       | errors => Js.log2("Errors: ", errors)
       };
       Js.Promise.resolve();
     })
  |> ignore;
};

let updateTeamPassword = (send, _event) => {
  let teamPassword =
    DomUtils.getValueOfInputById("invitations__team-password");
  send(UpdateTeamPassword(teamPassword));
};

let make = (~ctx, ~appSend, _children) => {
  ...component,
  initialState: () => {teamPassword: ""},
  reducer: (action, _state) =>
    switch (action) {
    | UpdateTeamPassword(tp) => ReasonReact.Update({teamPassword: tp})
    },
  render: ({state, send}) =>
    <div className="card mb-3">
      <div className="card-body">
        <h4 className="card-title">
          (str(ctx.invitation |> InvitationFromTeam.name))
        </h4>
        <h6 className="card-subtitle mb-2 text-muted">
          (str("from"))
          <code>
            (
              ctx.invitation
              |> InvitationFromTeam.email
              |> Email.toString
              |> str
            )
          </code>
        </h6>
        <p className="card-text">
          <input
            required=true
            id="invitations__team-password"
            className="w-100"
            _type="text"
            value=state.teamPassword
            onChange=(updateTeamPassword(send))
            placeholder="Enter your team's password to accept"
          />
        </p>
        <button
          onClick=(acceptInvitation(ctx, appSend))
          className="card-link btn btn-sm btn-success">
          (str("Accept"))
        </button>
        <button
          onClick=(rejectInvitation(ctx, appSend))
          className="card-link btn btn-sm btn-danger ml-2">
          (str("Reject"))
        </button>
      </div>
    </div>,
};