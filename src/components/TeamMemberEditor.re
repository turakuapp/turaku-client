let str = ReasonReact.string;
type ctx = {
  userData: Turaku.userData,
  teamMember: TeamMember.t,
};

let component = ReasonReact.statelessComponent("TeamMemberEditor");

let make = (~ctx: ctx, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div className="px-2">
      <div className="flex mt-2">
        <div className="w-32 mr-2" />
        <input
          className="w-1/2 text-lg p-2 bg-white hover:bg-grey-lighter focus:bg-grey-lighter font-bold"
          id="entry-editor__title"
          placeholder="Team Member's Name"
          _type="text"
          value=(ctx.teamMember |> TeamMember.name)
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
        />
      </div>
    </div>,
};