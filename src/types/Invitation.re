type t = {
  id,
  invitingTeam,
  invitingUser,
}
and id = int
and invitingTeam = {
  id: Team.id,
  name: string,
}
and invitingUser = {email: string};