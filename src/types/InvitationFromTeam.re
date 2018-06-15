type t = {
  id,
  teamName: string,
  invitingUserEmail: Email.t,
}
and id = string;

let email = invitation => invitation.invitingUserEmail;

let name = invitation => invitation.teamName;

let create = (id, ~teamName, ~invitingUserEmail) => {
  id,
  teamName,
  invitingUserEmail,
};