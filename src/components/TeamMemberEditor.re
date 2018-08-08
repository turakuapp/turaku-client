let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  teamMember: TeamMember.t,
};

type state = {name: string};

type action =
  | UpdateName(string);

let component = ReasonReact.reducerComponent("TeamMemberEditor");

let make = (~ctx: ctx, ~appSend, _children) => {
  ...component,
  initialState: () => {name: ctx.teamMember |> TeamMember.name},
  reducer: (action, _state) =>
    switch (action) {
    | UpdateName(name) => ReasonReact.Update({name: name})
    },
  render: ({state, send}) =>
    <div className="px-2">
      <div className="flex mt-2">
        <div className="w-32 mr-2" />
        <input
          className="w-1/2 text-lg p-2 bg-white hover:bg-grey-lighter focus:bg-grey-lighter font-bold"
          id="entry-editor__title"
          placeholder="Team Member's Name"
          _type="text"
          value=state.name
          onChange=(
            event => {
              let newValue = ReactDOMRe.domElementToObj(
                               ReactEventRe.Form.target(event),
                             )##value;
              send(UpdateName(newValue));
            }
          )
        />
      </div>
      <div className="flex mt-2">
        <div
          className="cursor-pointer w-32 font-thin hover:font-normal p-2 text-right mr-2">
          ("Email address" |> str)
        </div>
        <input
          className="w-1/2 p-2 rounded bg-white hover:bg-grey-lighter focus:bg-grey-lighter"
          id="entry-field__input-0"
          _type="text"
          value=(ctx.teamMember |> TeamMember.email |> Email.toString)
          readOnly=true
        />
      </div>
    </div>,
};