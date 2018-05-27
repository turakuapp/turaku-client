let saltPossibleCharacters = "1234567890abcdef";

/* TODO: This uses a potentially insecure randomization method. Use cryptographically secure alternative in production. */
let create = length => {
  let rec aux = (salt, remainingLength) =>
    if (remainingLength > 0) {
      let randomPosition = Random.int(String.length(saltPossibleCharacters));
      let saltCharacter =
        Js.String.charAt(randomPosition, saltPossibleCharacters);
      aux(salt ++ saltCharacter, remainingLength - 1);
    } else {
      salt;
    };
  aux("", length);
};