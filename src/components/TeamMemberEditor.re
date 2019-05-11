let str = React.string;

[@react.component]
let make = (~teamMember, ~appSend) => {
  let (name, setName) = React.useState(() => teamMember |> TeamMember.name);

  <div className="px-2">
    <div className="flex mt-2">
      <div className="w-32 mr-2" />
      <input
        className="w-1/2 text-lg p-2 bg-white hover:bg-grey-lighter focus:bg-grey-lighter font-bold"
        id="entry-editor__title"
        placeholder="Team Member's Name"
        type_="text"
        value=name
        onChange={event => setName(_ => ReactEvent.Form.target(event)##value)}
      />
    </div>
    <div className="flex mt-2">
      <div
        className="cursor-pointer w-32 font-thin hover:font-normal p-2 text-right mr-2">
        {"Email address" |> str}
      </div>
      <input
        className="w-1/2 p-2 rounded bg-white hover:bg-grey-lighter focus:bg-grey-lighter"
        id="entry-field__input-0"
        type_="text"
        value={teamMember |> TeamMember.email |> Email.toString}
        readOnly=true
      />
    </div>
  </div>;
};