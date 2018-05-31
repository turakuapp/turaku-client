exception GraphQLError(string);

let apiUrl = "http://turaku.localhost/graphql";

let fetchPost = (url, body) =>
  Fetch.fetchWithInit(
    url,
    Fetch.RequestInit.make(
      ~method_=Post,
      ~body=Fetch.BodyInit.make(body),
      ~headers=
        Fetch.HeadersInit.makeWithArray([|
          ("Content-Type", "application/json"),
        |]),
      (),
    ),
  );

let sendQuery = q =>
  Js.Dict.fromList([
    ("query", Js.Json.string(q##query)),
    ("variables", q##variables),
  ])
  |> Js.Json.object_
  |> Js.Json.stringify
  |> fetchPost(apiUrl)
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
         Js.Dict.unsafeGet(obj, "data") |> q##parse |> Js.Promise.resolve
       | None => Js.Promise.reject(GraphQLError("Response is not an object"))
       }
     );