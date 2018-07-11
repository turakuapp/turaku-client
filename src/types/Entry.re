type t =
  | Saved(entry, id)
  | Edited(entry, id, entry)
  | Unsaved(entry, id)
and entry = {
  title: string,
  fields: list(Field.t),
  tagIds: list(Tag.id),
}
and id = string;

let entry = t =>
  switch (t) {
  | Saved(entry, _)
  | Unsaved(entry, _)
  | Edited(_, _, entry) => entry
  };

module Codec = {
  let decode = (id, tagIds, json) =>
    Json.Decode.(
      Saved(
        {
          title: json |> field("title", string),
          fields: json |> field("fields", list(Field.Codec.decode)),
          tagIds,
        },
        id,
      )
    );

  let encode = t =>
    Json.Encode.(
      object_([
        ("title", string(entry(t).title)),
        ("fields", list(Field.Codec.encode, entry(t).fields)),
      ])
    );
};

let id = t =>
  switch (t) {
  | Saved(_, id)
  | Unsaved(_, id) => id
  | Edited(_, id, _) => id
  };

let title = t => entry(t).title;
let fields = t => entry(t).fields;
let tagIds = t => entry(t).tagIds;

let createEntry = (title, fields, tagIds) => {title, fields, tagIds};

let editEntry = (newEntry, t) =>
  switch (t) {
  | Saved(entry, id) => Edited(entry, id, newEntry)
  | Edited(original, id, _) =>
    if (original == newEntry) {
      Saved(newEntry, id);
    } else {
      Edited(original, id, newEntry);
    }
  | Unsaved(_, id) => Unsaved(newEntry, id)
  };

let editTitle = (title, t) => {
  let newEntry = createEntry(title, t |> fields, t |> tagIds);
  t |> editEntry(newEntry);
};

let replaceField = (field, index, fields) => {
  let rec aux = (c, idx, xs) =>
    switch (xs) {
    | [h, ...t] when idx == index => (c |> List.rev) @ [field, ...t]
    | [h, ...t] => aux([h, ...c], idx + 1, t)
    | [] => c |> List.rev
    };

  aux([], 0, fields);
};

let editField = (field, index, t) => {
  let newEntry =
    createEntry(
      t |> title,
      t |> fields |> replaceField(field, index),
      t |> tagIds,
    );
  t |> editEntry(newEntry);
};

let removeTag = (tagId, t) => {
  let newEntry =
    createEntry(
      t |> title,
      t |> fields,
      t |> tagIds |> List.filter(id => id != tagId),
    );
  t |> editEntry(newEntry);
};

let unpersisted = t =>
  switch (t) {
  | Saved(_) => false
  | Unsaved(_)
  | Edited(_) => true
  };

let newUnsaved = () =>
  Unsaved(
    {title: "New Entry", fields: Field.forNewEntry(), tagIds: []},
    "unsaved-" ++ (Js.Date.now() |> Js.Float.toString),
  );

let unsaved = t =>
  switch (t) {
  | Unsaved(_) => true
  | Saved(_)
  | Edited(_) => false
  };

let save = (id, t) =>
  switch (t) {
  | Saved(entry, _) => Saved(entry, id)
  | Edited(_original, _, edited) => Saved(edited, id)
  | Unsaved(entry, _) => Saved(entry, id)
  };