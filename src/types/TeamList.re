type t = {
  selected: Team.t,
  rest: list(Team.t),
};

let selected = t => t.selected;

let addTeam = (team, t) => {selected: team, rest: [t.selected, ...t.rest]};