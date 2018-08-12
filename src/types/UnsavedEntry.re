type t = {
  title: string,
  fields: list(Field.t),
};

let title = t => t.title;
let fields = t => t.fields;

let create = (title, fields) => {title, fields};
