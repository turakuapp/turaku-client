type t = {
  id,
  teamName: string,
  invitingUserEmail: string,
}
and id = Invitation.id;

let id = t => t.id;
let email = t => t.invitingUserEmail;
let name = t => t.teamName;

let create = (id, ~teamName, ~invitingUserEmail) => {
  id,
  teamName,
  invitingUserEmail,
};
