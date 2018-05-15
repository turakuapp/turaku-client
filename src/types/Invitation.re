type t = {
  id: int,
  invitingTeam,
  invitingUser,
}
and invitingTeam = {
  id: int,
  name: string,
}
and invitingUser = {email: string};

let decodeInvitingTeam = json =>
  Json.Decode.{
    id: json |> field("id", int),
    name: json |> field("name", string),
  };

let decodeInvitingUser = json =>
  Json.Decode.{email: json |> field("email", string)};

let decode = json =>
  Json.Decode.{
    id: json |> field("id", int),
    invitingTeam: json |> field("team", decodeInvitingTeam),
    invitingUser: json |> field("inviting_user", decodeInvitingUser),
  };

let email = invitation => invitation.invitingUser.email;

let name = invitation => invitation.invitingTeam.name;