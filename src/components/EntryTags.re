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

let myJsObject = {"foo": 1};

type tesla = {. color: string};

let obj: tesla = {val red = "Red"; pub color = red};

/* export default class EntryTags extends React.Component {
     constructor(props) {
       super(props);

       this.handleAddTag = this.handleAddTag.bind(this);
       this.handleDeleteTag = this.handleDeleteTag.bind(this);
       this.handleDragTag = this.handleDragTag.bind(this);
     }

     tagsForDisplay() {
       return _.map(this.props.entry.tags, tag => {
         if (_.isString(tag.name)) {
           return { text: tag.name };
         } else {
           return { id: tag.id, text: this.props.appState.tags[tag.id].name };
         }
       });
     }

     suggestedTags() {
       return _.map(this.props.appState.tags, tag => {
         return tag.name;
       });
     }

     async handleAddTag(tag) {
       // Create hash from tag text.
       const hashService = new HashService(
         tag.toLowerCase(),
         this.props.appState.team.password
       );

       const nameHash = await hashService.hexHash();

       // Check if client has tag in store.
       const savedTagId = _.findKey(this.props.appState.tags, tag => {
         return tag.nameHash === nameHash;
       });

       const savedTag = this.props.appState.tags[savedTagId];

       let newTag = null;

       if (_.isObject(savedTag)) {
         newTag = { id: savedTagId };
       } else {
         // when client does not have tag in store, encrypt tag text...
         const cryptoService = new CryptoService(
           this.props.appState.team.password,
           true
         );

         const encryptedTag = await cryptoService.encrypt(tag);
         newTag = { name: tag, encryptedName: encryptedTag, nameHash: nameHash };
       }

       // Now add the new tag to list of tags belonging to unsaved entry.
       const tagsClone = _.cloneDeep(this.props.entry.tags);
       tagsClone.push(newTag);

       this.saveChangeToTags(tagsClone);
     }

     handleDeleteTag(i) {
       // Remove the tag at supplied index.
       const tagsClone = _.cloneDeep(this.props.entry.tags);
       tagsClone.splice(i, 1);
       this.saveChangeToTags(tagsClone);
     }

     handleDragTag(tag, currPos, newPos) {
       const tagsClone = _.cloneDeep(this.props.entry.tags);

       // mutate array
       tagsClone.splice(currPos, 1);
       tagsClone.splice(newPos, 0, tag);

       // re-render
       this.saveChangeToTags(tagsClone);
     }

     saveChangeToTags(newTags) {
       const entryId = this.props.appState.entryId;
       const unsavedEntries = _.cloneDeep(this.props.appState.unsavedEntries);
       let unsavedEntry = unsavedEntries[entryId];

       if (!_.isObject(unsavedEntry)) {
         // The unsaved entry doesn't exist, copy it into the clone
         // of unsaved entries list from list of saved entries.
         unsavedEntry = _.cloneDeep(this.props.appState.entries[entryId]);
         unsavedEntries[entryId] = unsavedEntry;
       }

       unsavedEntry.tags = newTags;
       console.log("Setting updated unsavedEntries: ", unsavedEntries);
       this.props.setAppState({ unsavedEntries: unsavedEntries });
     }

     render() {
       return (
         <div className="row">
           <div className="col-2">Tags</div>
           <div className="col">
             <ReactTags
               tags={this.tagsForDisplay()}
               suggestions={this.suggestedTags()}
               handleDelete={this.handleDeleteTag}
               handleAddition={this.handleAddTag}
               handleDrag={this.handleDragTag}
             />
           </div>
         </div>
       );
     }
   } */