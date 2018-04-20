type baseUrl =
  | Some(string)
  | DefaultBaseUrl;

type t = {
  token: option(string),
  baseUrl,
};

type purpose =
  | SignUp;

type method =
  | Get
  | Post;

type headers = {
  .
  "Accept": string,
  "Content-Type": string,
  "Authorization": option(string),
};

type user = {
  id: int,
  name: string,
  email: string,
};

module DecodeSignUp = {
  let response = json =>
    Json.Decode.{
      id: json |> field("id", int),
      name: json |> field("name", string),
      email: json |> field("email", string),
    };
};

let decode = (p, json) =>
  switch (p) {
  | SignUp => DecodeSignUp.response(json)
  };

let path = p =>
  switch (p) {
  | SignUp => "users"
  };

let fetchPost = (url, body) =>
  Fetch.fetchWithInit(
    url,
    Fetch.RequestInit.make(
      ~method_=Post,
      ~body=Fetch.BodyInit.make(body),
      ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
      (),
    ),
  );

/* let cast(givenType: ) */
let fetch = (apiRequest, purpose, method, ~body=?, ()) => {
  let resolvedBaseUrl =
    switch (apiRequest.baseUrl) {
    | Some(url) => url
    | DefaultBaseUrl => "http://turaku.localhost/api/v0"
    };
  let fullRequestUrl = resolvedBaseUrl ++ "/" ++ path(purpose);
  Js.log("Calling " ++ fullRequestUrl);
  let requestBody =
    switch (body) {
    | Some(body) => body
    | None => "{}"
    };
  let fetch =
    switch (method) {
    | Post => fetchPost(fullRequestUrl, requestBody)
    | Get => Fetch.fetch(fullRequestUrl)
    };
  Js.Promise.(
    fetch
    |> then_(Fetch.Response.json)
    |> then_(json => json |> decode(purpose) |> resolve)
  );
  /* if (body !== null) {
       console.log(body);
             }

     return window
       .fetch(this.fullUrl(path), {
         method: method,
         headers: this.headers(),
         body: body
       })
       .then(this.parseResponse); */
};

let post = (p: purpose, params, apiRequest) => {
  let body = Js.Json.stringify(params);
  Js.log("Calling fetch for apiRequest with body " ++ body);
  fetch(apiRequest, p, Post, ~body, ());
};
/* export default class ApiService {
     constructor(token) {
       this.token = token;
       this.baseUrl = "http://turaku.localhost/api/v0";
     }

     headers() {
       let defaultHeaders = {
         Accept: "application/json",
         "Content-Type": "application/json"
       };

       if (this.token) {
         return _.merge(defaultHeaders, { Authorization: this.token });
       } else {
         return defaultHeaders;
       }
     }

     parseResponse(response) {
       let promise = response.json();

       // TODO: Check whether the response was a 4XX or a 5XX. A 4XX should be parsed, and a 5XX should be 'handled' with a server-failure response.
       if (response.ok) {
         return promise;
       } else {
         return promise.then(parsedResponse => Promise.reject(parsedResponse));
       }
     }

     fetch(path, method, body = null) {
       console.log("Calling " + this.fullUrl(path));

       if (body !== null) {
         console.log(body);
       }

       return window
         .fetch(this.fullUrl(path), {
           method: method,
           headers: this.headers(),
           body: body
         })
         .then(this.parseResponse);
     }

     get(path, params) {
       if (_.isObject(params)) {
         let incomingParams = {};

         if (_.includes(path, "?")) {
           incomingParams = _.split(path, "?")[1];
         }

         _.merge(incomingParams, params);
         path = path + "?" + queryString.stringify(incomingParams);
       }

       return this.fetch(path, "GET");
     }

     post(path, params) {
       let body = JSON.stringify(params);
       return this.fetch(path, "POST", body);
     }

     patch(path, params) {
       let body = JSON.stringify(params);
       return this.fetch(path, "PATCH", body);
     }

     delete(path) {
       return this.fetch(path, "DELETE");
     }

     fullUrl(path) {
       return [this.baseUrl, path].join("/");
     }
   } */