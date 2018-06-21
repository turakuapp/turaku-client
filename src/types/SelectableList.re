type t('a) = {
  selected: option('a),
  all: list('a),
};

let empty = () => {selected: None, all: []};

let all = t => t.all;

let selected = t => t.selected;

let select = (a, t) => {...t, selected: Some(a)};

let deselect = t => {...t, selected: None};

let add = (a, t) => {selected: Some(a), all: [a, ...t.all]};

let remove = (a, t) => {
  let selected = Some(a) == t.selected ? None : t.selected;
  {selected, all: t.all |> List.filter(b => b != a)};
};

let create = a =>
  switch (a) {
  | [h, ..._] => {selected: Some(h), all: a}
  | [] => empty()
  };

/** Replaces the first argument with the second in given SelectableList. */
let replace = (a, b, t) => {
  let selected = Some(a) == t.selected ? Some(b) : t.selected;
  {selected, all: t.all |> List.map(x => x == a ? b : x)};
};