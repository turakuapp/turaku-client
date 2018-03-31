let unwrapUnsafely = data =>
  switch (data) {
  | Some(v) => v
  | None => raise(Invalid_argument("unwrapUnsafely called on None"))
  };