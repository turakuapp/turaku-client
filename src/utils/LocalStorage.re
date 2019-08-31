module Team = {
  [@bs.deriving abstract]
  type t = {
    id: string,
    name: string,
    passwordIv: string,
    passwordCiphertext: string,
  };

  let make = t;

  [@bs.scope "JSON"] [@bs.val]
  external parseMany: string => array(t) = "parse";
};

module Session = {
  [@bs.deriving abstract]
  type t = {
    token: string,
    encryptionSalt: string,
  };

  let make = t;

  [@bs.scope "JSON"] [@bs.val] external parse: string => t = "parse";
};

module Entry = {
  [@bs.deriving abstract]
  type t = {
    id: string,
    dataIv: string,
    dataCiphertext: string,
    tagIds: array(string),
  };

  let make = t;

  [@bs.scope "JSON"] [@bs.val]
  external parseMany: string => array(t) = "parse";
};

let unsafeStringify = x =>
  x |> Js.Json.stringifyAny |> OptionUtils.unwrapUnsafely;

let save = (key, value) =>
  Dom.Storage.setItem(key, value, Dom.Storage.sessionStorage);

let saveTeams = (teams: array(Team.t)) =>
  save("teams", teams |> unsafeStringify);

let saveSession = (session: Session.t) =>
  save("session", session |> unsafeStringify);

let entriesKey = teamId => "teams." ++ teamId ++ "." ++ "entries";

let saveEntries = (teamId, entries: array(Entry.t)) =>
  save(teamId |> entriesKey, entries |> unsafeStringify);

let getItem = Dom.Storage.getItem(_, Dom.Storage.sessionStorage);

let loadSession = () =>
  switch (getItem("session")) {
  | Some(session) => Some(session |> Session.parse)
  | None => None
  };

let loadTeams = () =>
  switch (getItem("teams")) {
  | Some(teams) => teams |> Team.parseMany
  | None => [||]
  };

let loadEntrys = teamId =>
  switch (teamId |> entriesKey |> getItem) {
  | Some(entries) => entries |> Entry.parseMany
  | None => [||]
  };
