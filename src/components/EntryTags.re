exception CreateTagException(array(string));

let str = React.string;

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

let createTag = (team, session, name, nameHash) => {
  let key = team |> Team.createCryptographicKey;
  name
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
         Tag.create(tag##id, ~name, ~nameHash) |> Js.Promise.resolve
       | None =>
         let errors = response##createTag##errors;
         Js.log2("createTag errors: ", errors);
         Js.Promise.reject(CreateTagException(errors));
       }
     );
};

let addTag = (team, session, name, setName, appSend, entry, event) => {
  event |> DomUtils.preventEventDefault;

  Hash.create(name |> Js.String.toUpperCase, Salt.empty())
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
       | None => createTag(team, session, name, nameHash)
       };
     })
  |> Js.Promise.then_(tag => {
       appSend(Turaku.AddTagToEntry(entry, tag));
       setName(_ => "");
       Js.Promise.resolve();
     })
  |> ignore;
};

[@react.component]
let make = (~team, ~entry, ~session, ~appSend, _children) => {
  let (name, setName) = React.useState(() => "");
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
      <form onSubmit={addTag(team, session, name, setName, appSend, entry)}>
        <input
          className="mt-2"
          type_="text"
          onChange={
            event => setName(_ => event->ReactEvent.Form.target##value)
          }
          value=name
          placeholder="Add tags"
          onBlur={_e => setName(_ => "")}
        />
      </form>
      <TagOptions team entry search={state.name} />
    </div>
  </div>;
};