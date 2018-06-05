[@bs.send]
external preventEventDefault : ReactEventRe.Form.t => unit = "preventDefault";

let getValueOfInputById = id => {
  let inputElement =
    Webapi.Dom.document |> Webapi.Dom.Document.getElementById(id);
  switch (inputElement) {
  | Some(element) =>
    element
    |> Webapi.Dom.Element.asHtmlElement
    |> Belt.Option.getExn
    |> Webapi.Dom.HtmlElement.value
  | None => ""
  };
};