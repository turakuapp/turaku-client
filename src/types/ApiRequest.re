type requestMethod =
  | Get
  | Post
  | Put
  | Patch;

type baseUrl =
  | Some(string)
  | DefaultBaseUrl;

type t = {
  token: option(string),
  baseUrl
};

type headers = {
  .
  "Accept": string,
  "Content-Type": string,
  "Authorization": option(string)
};

let fetch = (apiRequest, path, method, ~body=?, ()) => {
  let baseUrl =
    switch apiRequest.baseUrl {
    | Some(url) => url
    | DefaultBaseUrl => "http://turaku.localhost/api/v0"
    };
  let fullUrl = baseUrl ++ "/" ++ path;
  Js.log("Calling " ++ fullUrl);
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

let post = (path, params, apiRequest) => {
  let body = Js.Json.stringify(params);
  fetch(apiRequest, path, Post, ~body);
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