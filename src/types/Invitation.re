type id = string;

module DeleteInvitationQuery = [%graphql
  {|
  mutation($id: ID!) {
    deleteInvitation(id: $id) {
      errors
    }
  }
  |}
];

let delete = (session, id: id) =>
  DeleteInvitationQuery.make(~id, ()) |> Api.sendAuthenticatedQuery(session);