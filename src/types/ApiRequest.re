type baseUrl =
  | DefaultBaseUrl
  | CustomBaseUrl(string);

type purpose =
  | SignUp
  | SignIn;

type method =
  | Get
  | Post;

type t = {
  token: option(string),
  baseUrl,
  purpose,
};

type headers = {
  .
  "Accept": string,
  "Content-Type": string,
  "Authorization": option(string),
};

let create = (~purpose, ~token=?, ()) => {
  token,
  baseUrl: DefaultBaseUrl,
  purpose,
};

let path = p =>
  switch (p) {
  | SignUp => "users"
  | SignIn => "sessions"
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

let methodForPurpose = purpose =>
  switch (purpose) {
  | SignIn
  | SignUp => Post
  };

/* let cast(givenType: ) */
let fetch = (~apiRequest, ~params=?, ()) => {
  let resolvedBaseUrl =
    switch (apiRequest.baseUrl) {
    | CustomBaseUrl(url) => url
    | DefaultBaseUrl => "http://turaku.localhost/api/v0"
    };
  let fullRequestUrl = resolvedBaseUrl ++ "/" ++ path(apiRequest.purpose);
  Js.log("Calling " ++ fullRequestUrl);
  let requestBody =
    switch (params) {
    | Some(p) => Js.Json.stringify(p)
    | None => "{}"
    };
  let performedFetch =
    switch (apiRequest.purpose |> methodForPurpose) {
    | Post => fetchPost(fullRequestUrl, requestBody)
    | Get => Fetch.fetch(fullRequestUrl)
    };
  performedFetch |> Js.Promise.then_(Fetch.Response.json);
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