type t;
type id = Invitation.id;

let id: t => id;
let email: t => string;
let name: t => string;

let create: (id, ~teamName: string, ~invitingUserEmail: string) => t;
