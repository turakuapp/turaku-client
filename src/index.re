[%bs.raw {|require("bootstrap/dist/css/bootstrap.css")|}];

[@bs.module "./registerServiceWorker"]
external register_service_worker : unit => unit = "default";

ReactDOMRe.renderToElementWithId(
  <ReasonApollo.Provider client=GraphQLClient.instance>
    <Root />
  </ReasonApollo.Provider>,
  "root",
);

register_service_worker();