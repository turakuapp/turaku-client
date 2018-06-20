type t = {
  selected: InvitationToUser.t,
  rest: list(InvitationToUser.t),
};

let selected = t => t.selected;

let addInvitation = (invitation, t) => {
  selected: invitation,
  rest: [t.selected, ...t.rest],
};