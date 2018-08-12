let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  team: Team.t,
  entry: Entry.t,
  field: Field.t,
  index: int,
};

type state = {reveal: bool};

type action =
  | ForceReveal
  | UnforceReveal;

let component = ReasonReact.reducerComponent("EntryField");

let fieldValue = ctx => ctx.field |> Field.value;

let id = ctx => "entry-field__input-" ++ (ctx.index |> string_of_int);

let editField = (ctx, appSend, _event) => {
  Js.log("Editing field with index: " ++ string_of_int(ctx.index));
  let v = id(ctx) |> DomUtils.getValueOfInputById;
  appSend(
    Turaku.EditEntryField(ctx.field |> Field.editValue(v), ctx.index),
  );
};

let fieldType = (ctx, state) =>
  switch (state.reveal, ctx.field |> Field.shouldBeHidden) {
  | (true, true)
  | (false, false)
  | (true, false) => "text"
  | (false, true) => "password"
  };

let make = (~ctx, ~appSend, _children) => {
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
         (ctx.field |> Field.key |> str) </div>
      <input
        className="w-1/2 p-2 rounded bg-white hover:bg-grey-lighter focus:bg-grey-lighter"
        value=(fieldValue(ctx))
        type_=(fieldType(ctx, state))
        onChange=(editField(ctx, appSend))
        id=(id(ctx))
        onFocus=(_e => send(ForceReveal))
        onBlur=(_e => send(UnforceReveal))
      />
    </div>,
};
