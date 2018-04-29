type t = {
  id,
  title: string,
  fields: list(Field.t),
  persisted: bool,
}
and id = int;