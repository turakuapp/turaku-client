type entry = {
  title: string,
  fields: array(Field.t),
};

type id = string;

type t =
  | Saved(entry, id)
  | Edited(entry, id, entry)
  | Unsaved(entry, id);

module Codec = {
  let decode = (id, json) =>
    Json.Decode.(
      Saved(
        {
          title: json |> field("title", string),
          fields: json |> field("fields", array(Field.Codec.decode)),
        },
        id,
      )
    );
};

let entry = t =>
  switch (t) {
  | Saved(entry, _)
  | Unsaved(entry, _) => entry
  | Edited(_, _, entry) => entry
  };

let id = t =>
  switch (t) {
  | Saved(_, id)
  | Unsaved(_, id) => id
  | Edited(_, id, _) => id
  };

let title = t => entry(t).title;
let fields = t => entry(t).fields;

let createEntry = (title, fields) => {title, fields};

let editEntry = (newEntry, t) =>
  switch (t) {
  | Saved(entry, id) => Edited(entry, id, newEntry)
  | Edited(original, id, _) => Edited(original, id, newEntry)
  | Unsaved(_, id) => Unsaved(newEntry, id)
  };

let editTitle = (title, t) => {
  let newEntry = createEntry(title, t |> fields);
  t |> editEntry(newEntry);
};

let replaceField = (field, index, fields) => {
  fields[index] = field;
  fields;
};

let editField = (field, index, t) => {
  let newEntry =
    createEntry(t |> title, t |> fields |> replaceField(field, index));
  t |> editEntry(newEntry);
};

let unpersisted = t =>
  switch (t) {
  | Saved(_) => false
  | Unsaved(_)
  | Edited(_) => true
  };