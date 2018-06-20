type t = {
  selected: TeamMember.t,
  rest: list(TeamMember.t),
};

let selected = t => t.selected;