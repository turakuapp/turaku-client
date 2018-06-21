type t('a) = {
  selected: option('a),
  rest: list('a),
};

let empty = () => {selected: None, rest: []};

let all = t =>
  switch (t.selected) {
  | None => t.rest
  | Some(a) => [a, ...t.rest]
  };

let selected = t => t.selected;

let select = (a, t) => {
  selected: Some(a),
  rest: t |> all |> List.filter(b => b != a),
};

let deselect = t => {
  selected: None,
  rest:
    switch (t.selected) {
    | None => t.rest
    | Some(a) => [a, ...t.rest]
    },
};

let add = (a, t) => {selected: Some(a), rest: t |> all};

let remove = (a, t) =>
  if (Some(a) == t.selected) {
    {...t, selected: None};
  } else {
    {...t, rest: t.rest |> List.filter(b => b != a)};
  };

let create = a =>
  switch (a) {
  | [h, ...t] => {selected: Some(h), rest: t}
  | [] => {selected: None, rest: []}
  };

/** Replaces the first argument with the second in given SelectableList. */
let replace = (a, b, t) =>
  if (Some(a) == t.selected) {
    {...t, selected: Some(b)};
  } else {
    {...t, rest: t.rest |> List.map(x => x == a ? b : x)};
  };