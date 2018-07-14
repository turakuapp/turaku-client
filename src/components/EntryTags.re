exception CreateTagException(array(string));

let str = ReasonReact.string;

type state = {name: string};

type action =
  | UpdateName(string)
  | AddNewTag(Tag.t);

type ctx = {
  team: Team.t,
  entry: Entry.t,
  session: Session.t,
};

let component = ReasonReact.reducerComponent("EntryTags");

let updateName = (send, event) => {
  let targetObject =
    event |> ReactEventRe.Form.target |> ReactDOMRe.domElementToObj;
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

let createTag = (ctx, state, nameHash) => {
  let key = ctx.team |> Team.createCryptographicKey;
  state.name
  |> EncryptedData.encrypt(key)
  |> Js.Promise.then_(encryptedName =>
       CreateTagQuery.make(
         ~teamId=ctx.team |> Team.id,
         ~nameHash,
         ~iv=
           encryptedName |> EncryptedData.iv |> InitializationVector.toString,
         ~ciphertext=
           encryptedName |> EncryptedData.ciphertext |> CipherText.toString,
         (),
       )
       |> Api.sendAuthenticatedQuery(ctx.session)
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

let addTag = (ctx, state, send, event) => {
  event |> DomUtils.preventEventDefault;

  Hash.create(state.name |> Js.String.toUpperCase, Salt.empty())
  |> Js.Promise.then_(hash => {
       let nameHash =
         hash
         |> UnsignedByteArray.fromArrayBuffer
         |> UnsignedByteArray.toBase64String;
       let someTag =
         ctx.team
         |> Team.tags
         |> SelectableList.all
         |> ListUtils.find_opt(tag => tag |> Tag.nameHash == nameHash);

       switch (someTag) {
       | Some(tag) => tag |> Js.Promise.resolve
       | None => createTag(ctx, state, nameHash)
       };
     })
  |> Js.Promise.then_(tag => {
       send(AddNewTag(tag));
       Js.Promise.resolve();
     })
  |> ignore;
};

let make = (~ctx, ~appSend, _children) => {
  ...component,
  initialState: () => {name: ""},
  reducer: (action, _state) =>
    switch (action) {
    | UpdateName(name) => ReasonReact.Update({name: name})
    | AddNewTag(tag) =>
      appSend(AddTagToEntry(ctx.entry, tag));
      ReasonReact.Update({name: ""});
    },
  render: ({state, send}) =>
    <div className="row">
      <div className="col-sm-2 font-weight-bold"> ("Tags" |> str) </div>
      <div className="col">
        <div className="mt-1">
          (
            ctx.entry
            |> Entry.tagIds
            |> List.map(tagId =>
                 <EntryTag
                   key=tagId
                   team=ctx.team
                   entry=ctx.entry
                   tagId
                   appSend
                 />
               )
            |> Array.of_list
            |> ReasonReact.array
          )
        </div>
        <form onSubmit=(addTag(ctx, state, send))>
          <input
            className="mt-2"
            type_="text"
            onChange=(updateName(send))
            value=state.name
            placeholder="Add tags"
            onBlur=(_e => send(UpdateName("")))
          />
        </form>
        <TagOptions team=ctx.team entry=ctx.entry search=state.name />
      </div>
    </div>,
};