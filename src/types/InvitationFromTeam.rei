type t;
type id = Invitation.id;

let id: t => id;
let email: t => Email.t;
let name: t => string;

let create: (id, ~teamName: string, ~invitingUserEmail: Email.t) => t;