type kind =
  | PasswordField
  | TextField;

type t = {
  kind,
  key,
  value,
}
and key = string
and value = string;