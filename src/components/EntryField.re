let str = ReasonReact.string;

type state = {reveal: bool};

type action =
  | ForceReveal
  | UnforceReveal;

let component = ReasonReact.reducerComponent("EntryField");

let id = index => "entry-field__input-" ++ (index |> string_of_int);

let editField = (field, index, appSend, _event) => {
  Js.log("Editing field with index: " ++ string_of_int(index));
  let v = id(index) |> DomUtils.getValueOfInputById;
  appSend(Turaku.EditEntryField(field |> Field.editValue(v), index));
};

let fieldType = (field, state) =>
  switch (state.reveal, field |> Field.shouldBeHidden) {
  | (true, true)
  | (false, false)
  | (true, false) => "text"
  | (false, true) => "password"
  };

let make = (~entry, ~field, ~index, ~appSend, _children) => {
  ...component,
  initialState: () => {reveal: false},
  reducer: (action, _state) =>
    switch (action) {
    | ForceReveal => ReasonReact.Update({reveal: true})
    | UnforceReveal => ReasonReact.Update({reveal: false})
    },
  render: ({state, send}) =>
    <div className="flex mt-1">
      <div
        className="cursor-pointer w-32 font-thin hover:font-normal p-2 text-right mr-2">
        /* TODO: Clicking the field name should copy the contents */
         {field |> Field.key |> str} </div>
      <input
        className="w-1/2 p-2 rounded bg-white hover:bg-grey-lighter focus:bg-grey-lighter"
        value={field |> Field.value}
        type_={fieldType(field, state)}
        onChange={editField(field, index, appSend)}
        id={id(index)}
        onFocus={_e => send(ForceReveal)}
        onBlur={_e => send(UnforceReveal)}
      />
    </div>,
};