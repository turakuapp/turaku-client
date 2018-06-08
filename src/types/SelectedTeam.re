type t = {
  id: Team.id,
  password: TeamPassword.t,
};

let create = (password, id) => {id, password};

let getId = t => t.id;

let getCryptographicKey = t =>
  t.password |> CryptographicKey.keyFromTeamPassword;