exception CreateTagException(array(string));

let str = ReasonReact.string;

type state = {name: string};

type action =
  | UpdateName(string)
  | AddNewTag(Tag.t);

let component = ReasonReact.reducerComponent("EntryTags");

let updateName = (send, event) => {
  let targetObject = event->ReactEvent.Form.target;
  send(UpdateName(targetObject##value));
};

module CreateTagQuery = [%graphql
  {|
  mutation($teamId: ID!, $nameHash: String!, $iv: String!, $ciphertext: String!) {
    createTag(teamId: $teamId, nameHash: $nameHash, encryptedName: {iv: $iv, ciphertext: $ciphertext}) {
      tag {
        id
      }
      errors
    }
  }
  |}
];

let createTag = (team, session, state, nameHash) => {
  let key = team |> Team.createCryptographicKey;
  state.name
  |> EncryptedData.encrypt(key)
  |> Js.Promise.then_(encryptedName =>
       CreateTagQuery.make(
         ~teamId=team |> Team.id,
         ~nameHash,
         ~iv=
           encryptedName |> EncryptedData.iv |> InitializationVector.toString,
         ~ciphertext=
           encryptedName |> EncryptedData.ciphertext |> CipherText.toString,
         (),
       )
       |> Api.sendAuthenticatedQuery(session)
     )
  |> Js.Promise.then_(response =>
       switch (response##createTag##tag) {
       | Some(tag) =>
         Tag.create(tag##id, ~name=state.name, ~nameHash)
         |> Js.Promise.resolve
       | None =>
         let errors = response##createTag##errors;
         Js.log2("createTag errors: ", errors);
         Js.Promise.reject(CreateTagException(errors));
       }
     );
};

let addTag = (team, session, state, send, event) => {
  event |> DomUtils.preventEventDefault;

  Hash.create(state.name |> Js.String.toUpperCase, Salt.empty())
  |> Js.Promise.then_(hash => {
       let nameHash =
         hash
         |> UnsignedByteArray.fromArrayBuffer
         |> UnsignedByteArray.toBase64String;
       let someTag =
         team
         |> Team.tags
         |> SelectableList.all
         |> ListUtils.find_opt(tag => tag |> Tag.nameHash == nameHash);

       switch (someTag) {
       | Some(tag) => tag |> Js.Promise.resolve
       | None => createTag(team, session, state, nameHash)
       };
     })
  |> Js.Promise.then_(tag => {
       send(AddNewTag(tag));
       Js.Promise.resolve();
     })
  |> ignore;
};

let make = (~team, ~entry, ~session, ~appSend, _children) => {
  ...component,
  initialState: () => {name: ""},
  reducer: (action, _state) =>
    switch (action) {
    | UpdateName(name) => ReasonReact.Update({name: name})
    | AddNewTag(tag) =>
      appSend(AddTagToEntry(entry, tag));
      ReasonReact.Update({name: ""});
    },
  render: ({state, send}) =>
    <div className="flex mt-1">
      <div
        className="cursor-pointer w-32 font-thin hover:font-normal p-2 text-right mr-2">
        {"Tags" |> str}
      </div>
      <div className="col">
        <div className="mt-1">
          {
            entry
            |> Entry.tagIds
            |> List.map(tagId => <EntryTag team entry tagId appSend />)
            |> Array.of_list
            |> ReasonReact.array
          }
        </div>
        <form onSubmit={addTag(team, session, state, send)}>
          <input
            className="mt-2"
            type_="text"
            onChange={updateName(send)}
            value={state.name}
            placeholder="Add tags"
            onBlur={_e => send(UpdateName(""))}
          />
        </form>
        <TagOptions team entry search={state.name} />
      </div>
    </div>,
};