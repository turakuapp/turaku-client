type t = {
  id,
  invitingTeam,
  invitingUser,
}
and invitingTeam = {name: string}
and invitingUser = {email: string}
and id = string;

let email = invitation => invitation.invitingUser.email;

let name = invitation => invitation.invitingTeam.name;

let create = (id, ~teamName, ~userEmail) => {
  id,
  invitingTeam: {
    name: teamName,
  },
  invitingUser: {
    email: userEmail,
  },
};