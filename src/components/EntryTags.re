let str = ReasonReact.string;

type state = {name: string};

type action =
  | UpdateName(string);

type retainedProps = {entry: Entry.t};

let component = ReasonReact.reducerComponentWithRetainedProps("EntryTags");

let updateName = (send, event) => {
  let name = ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value;
  send(UpdateName(name));
};

let make = (~team, ~entry, ~appSend, _children) => {
  ...component,
  initialState: () => {name: ""},
  retainedProps: {
    entry: entry,
  },
  didUpdate: ({oldSelf, newSelf}) =>
    if (oldSelf.retainedProps.entry
        |> Entry.id != (newSelf.retainedProps.entry |> Entry.id)
        && newSelf.state.name != "") {
      newSelf.send(UpdateName(""));
    },
  reducer: (action, state) =>
    switch (action) {
    | UpdateName(name) => ReasonReact.Update({name: name})
    },
  render: ({state, send}) =>
    <div className="row">
      <div className="col-sm-2 font-weight-bold"> ("Tags" |> str) </div>
      <div className="col">
        <div className="mt-1">
          (
            entry
            |> Entry.tagIds
            |> List.map(tagId => <EntryTag key=tagId team tagId />)
            |> Array.of_list
            |> ReasonReact.array
          )
        </div>
        <input
          className="mt-2"
          type_="text"
          onChange=(updateName(send))
          value=state.name
          placeholder="Add tags"
        />
        <TagOptions team entry search=state.name />
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