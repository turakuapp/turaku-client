exception UnsafeUnwrapFailed;

let unwrapUnsafely = o =>
  switch (o) {
  | Some(v) => v
  | None => raise(UnsafeUnwrapFailed)
  };

let toBool = o =>
  switch (o) {
  | Some(_) => true
  | None => false
  };
