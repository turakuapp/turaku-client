type id = string;

let delete:
  (Session.t, id) =>
  Js.Promise.t({. "deleteInvitation": {. "errors": Js.Array.t(string)}});