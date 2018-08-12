let find_opt = (p, l) =>
  try (Some(List.find(p, l))) {
  | Not_found => None
  };
