type t = {
  selected: Entry.t,
  rest: list(Entry.t),
};

let selected = t => t.selected;