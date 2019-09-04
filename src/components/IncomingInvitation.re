let str = React.string;

module AcceptInvitationQuery = [%graphql
  {|
  mutation($id: ID!, $iv: String!, $ciphertext: String!) {
    acceptInvitation(id: $id, encryptedPassword: {iv: $iv, ciphertext: $ciphertext}) {
      team {
        id
      }
      errors
    }
  }
  |}
];

let acceptInvitation = (session, invitation, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;

  let invitationId = invitation |> InvitationFromTeam.id;
  Js.log("Accepting Invitation#" ++ invitationId);

  let stringPassword =
    DomUtils.getValueOfInputById("invitations__team-password");

  let encryptionKey = session |> Session.getCryptographicKey;
  EncryptedData.encrypt(encryptionKey, stringPassword)
  |> Js.Promise.then_(encryptedData => {
       let iv =
         encryptedData |> EncryptedData.iv |> InitializationVector.toString;
       let ciphertext =
         encryptedData |> EncryptedData.ciphertext |> CipherText.toString;

       AcceptInvitationQuery.make(
         ~id=invitation |> InvitationFromTeam.id,
         ~iv,
         ~ciphertext,
         (),
       )
       |> Api.sendAuthenticatedQuery(session);
     })
  |> Js.Promise.then_(response => {
       switch (response##acceptInvitation##team) {
       | Some(invitingTeam) =>
         let team =
           Team.create(
             invitingTeam##id,
             invitation |> InvitationFromTeam.name,
             stringPassword |> TeamPassword.fromString,
           );
         appSend(Turaku.AcceptInvitation(team, invitation));
       | None => Js.log2("Errors: ", response##acceptInvitation##errors)
       };
       Js.Promise.resolve();
     })
  |> ignore;
};

let rejectInvitation = (session, invitation, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;

  let invitationId = invitation |> InvitationFromTeam.id;
  Js.log("Rejecting Invitation#" ++ invitationId);

  invitationId
  |> Invitation.delete(session)
  |> Js.Promise.then_(response => {
       switch (response##deleteInvitation##errors |> Array.to_list) {
       | [] => appSend(Turaku.RemoveInvitationFromTeam(invitation))
       | errors => Js.log2("Errors: ", errors)
       };
       Js.Promise.resolve();
     })
  |> ignore;
};

[@react.component]
let make = (~session, ~invitation, ~appSend) => {
  let (teamPassword, setTeamPassword) = React.useState(() => "");
  <div className="card mb-3">
    <div className="card-body">
      <h4 className="card-title">
        {str(invitation |> InvitationFromTeam.name)}
      </h4>
      <h6 className="card-subtitle mb-2 text-muted">
        {str("from")}
        <code> {invitation |> InvitationFromTeam.email |> str} </code>
      </h6>
      <p className="card-text">
        <input
          required=true
          className="w-100"
          type_="text"
          value=teamPassword
          onChange={
            event => setTeamPassword(_ => event->ReactEvent.Form.target##value)
          }
          placeholder="Enter your team's password to accept"
        />
      </p>
      <button
        onClick={acceptInvitation(session, invitation, appSend)}
        className="card-link btn btn-sm btn-success">
        {str("Accept")}
      </button>
      <button
        onClick={rejectInvitation(session, invitation, appSend)}
        className="card-link btn btn-sm btn-danger ml-2">
        {str("Reject")}
      </button>
    </div>
  </div>;
};
