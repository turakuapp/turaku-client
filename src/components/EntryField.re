let str = React.string;

let id = index => "entry-field__input-" ++ (index |> string_of_int);

let editField = (field, index, appSend, _event) => {
  Js.log("Editing field with index: " ++ string_of_int(index));
  let v = id(index) |> DomUtils.getValueOfInputById;
  appSend(Turaku.EditEntryField(field |> Field.editValue(v), index));
};

let fieldType = (field, hidden) =>
  switch (hidden, field |> Field.shouldBeHidden) {
  | (true, true) => "password"
  | (false, true)
  | (_, false) => "text"
  };

[@react.component]
let make = (~entry, ~field, ~index, ~appSend, _children) => {
  let (hidden, setHidden) = React.useState(() => true);

  <div className="flex mt-1">
    <div
      className="cursor-pointer w-32 font-thin hover:font-normal p-2 text-right mr-2">
      /* TODO: Clicking the field name should copy the contents */
       {field |> Field.key |> str} </div>
    <input
      className="w-1/2 p-2 rounded bg-white hover:bg-grey-lighter focus:bg-grey-lighter"
      value={field |> Field.value}
      type_={fieldType(field, hidden)}
      onChange={editField(field, index, appSend)}
      id={id(index)}
      onFocus={_e => setHidden(_ => false)}
      onBlur={_e => setHidden(_ => true)}
    />
  </div>;
};