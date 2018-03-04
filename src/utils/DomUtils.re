[@bs.send]
external preventEventDefault : ReactEventRe.Form.t => unit = "preventDefault";

let optionAttributeToString = optionalAttribute =>
  switch optionalAttribute {
  | Some(attribute) => attribute
  | None => ""
  };

let unwrapUnsafely = data =>
  switch data {
  | Some(v) => v
  | None => raise(Invalid_argument("unwrapUnsafely called on None"))
  };

let getValueOfInputById = id => {
  let inputElement =
    Webapi.Dom.document |> Webapi.Dom.Document.getElementById(id);
  switch inputElement {
  | Some(element) =>
    element
    |> Webapi.Dom.Element.asHtmlElement
    |> unwrapUnsafely
    |> Webapi.Dom.HtmlElement.value
  | None => ""
  };
};