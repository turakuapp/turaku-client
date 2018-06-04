exception GraphQLError(string);

let apiUrl = "http://turaku.localhost/graphql";

let fetchPost = (url, accessToken: option(AccessToken.t), body) => {
  let defaultHeaders = [("Content-Type", "application/json")];
  let headers =
    switch (accessToken) {
    | None => defaultHeaders
    | Some(accessToken) => [
        ("Authorization", accessToken),
        ...defaultHeaders,
      ]
    };
  Fetch.fetchWithInit(
    url,
    Fetch.RequestInit.make(
      ~method_=Post,
      ~body=Fetch.BodyInit.make(body),
      ~headers=Fetch.HeadersInit.makeWithArray(headers |> Array.of_list),
      (),
    ),
  );
};

let sendQuery = (session, query) => {
  let credentials = session |> Session.getCredentials;
  let accessToken =
    switch (credentials) {
    | None => None
    | Some(c) => Some(c |> Session.getAccessToken)
    };
  Js.Dict.fromList([
    ("query", Js.Json.string(query##query)),
    ("variables", query##variables),
  ])
  |> Js.Json.object_
  |> Js.Json.stringify
  |> fetchPost(apiUrl, accessToken)
  |> Js.Promise.then_(response =>
       if (Fetch.Response.ok(response)) {
         Fetch.Response.json(response);
       } else {
         Js.Promise.reject(
           GraphQLError(
             "Request failed: " ++ Fetch.Response.statusText(response),
           ),
         );
       }
     )
  |> Js.Promise.then_(json =>
       switch (Js.Json.decodeObject(json)) {
       | Some(obj) =>
         Js.Dict.unsafeGet(obj, "data") |> query##parse |> Js.Promise.resolve
       | None => Js.Promise.reject(GraphQLError("Response is not an object"))
       }
     );
};