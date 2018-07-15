[%bs.raw {|require("./index.css")|}];

[@bs.module "./registerServiceWorker"]
external register_service_worker : unit => unit = "default";

ReactDOMRe.renderToElementWithId(<Root />, "root");

register_service_worker();