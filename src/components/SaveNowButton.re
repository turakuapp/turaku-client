exception CreateFailure(string);

let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  team: Team.t,
  entry: Entry.t,
};

type state = {saving: bool};

type action =
  | Save;

let component = ReasonReact.reducerComponent("SaveNowButton");

module CreateEntryQuery = [%graphql
  {|
  mutation($teamId: ID!, $iv: String!, $ciphertext: String!, $tagIds: [ID!]!) {
    createEntry(teamId: $teamId, encryptedData: {iv: $iv, ciphertext: $ciphertext}, tagIds: $tagIds) {
      entry {
        id
      }
      errors
    }
  }
  |}
];

module UpdateEntryQuery = [%graphql
  {|
  mutation($id: ID!, $iv: String!, $ciphertext: String!, $tagIds: [ID!]!) {
    updateEntry(id: $id, encryptedData: {iv: $iv, ciphertext: $ciphertext}, tagIds: $tagIds) {
      entry {
        id
      }
      errors
    }
  }
  |}
];

let saveChangesInBackground = (ctx, appSend) => {
  let key = ctx.team |> Team.createCryptographicKey;
  ctx.entry
  |> Entry.Codec.encode
  |> Js.Json.stringify
  |> EncryptedData.encrypt(key)
  |> Js.Promise.then_(encryptedData =>
       if (ctx.entry |> Entry.unsaved) {
         CreateEntryQuery.make(
           ~teamId=ctx.team |> Team.id,
           ~iv=
             encryptedData |> EncryptedData.iv |> InitializationVector.toString,
           ~ciphertext=
             encryptedData |> EncryptedData.ciphertext |> CipherText.toString,
           ~tagIds=[||],
           (),
         )
         |> Api.sendAuthenticatedQuery(ctx.userData.session)
         |> Js.Promise.then_(response => {
              let updatedEntry = response##createEntry##entry;
              switch (updatedEntry) {
              | Some(e) =>
                ctx.entry |> Entry.save(e##id) |> Js.Promise.resolve
              | None => CreateFailure("") |> Js.Promise.reject
              };
            });
       } else {
         UpdateEntryQuery.make(
           ~id=ctx.entry |> Entry.id,
           ~iv=
             encryptedData |> EncryptedData.iv |> InitializationVector.toString,
           ~ciphertext=
             encryptedData |> EncryptedData.ciphertext |> CipherText.toString,
           ~tagIds=[||],
           (),
         )
         |> Api.sendAuthenticatedQuery(ctx.userData.session)
         |> Js.Promise.then_(_response =>
              ctx.entry
              |> Entry.save(ctx.entry |> Entry.id)
              |> Js.Promise.resolve
            );
       }
     )
  |> Js.Promise.then_(updatedEntry => {
       appSend(Turaku.ReplaceEntry(ctx.team, ctx.entry, updatedEntry));
       Js.Promise.resolve();
     })
  |> ignore;
};

let saveChanges = (ctx, send, appSend) => {
  Js.log("Saving entry: " ++ (ctx.entry |> Entry.id));
  send(Save);
  saveChangesInBackground(ctx, appSend);
};

let make = (~ctx, ~appSend, _children) => {
  ...component,
  willUnmount: ({state}) =>
    if (ctx.entry |> Entry.unpersisted && ! state.saving) {
      Js.log(
        "Auto-saving (on unmount) entry with ID: " ++ (ctx.entry |> Entry.id),
      );
      saveChangesInBackground(ctx, appSend);
    },
  didUpdate: ({newSelf}) => {
    let selectedEntry = ctx.team |> Team.entries |> SelectableList.selected;
    if (ctx.entry
        |> Entry.unpersisted
        && Some(ctx.entry) != selectedEntry
        && ! newSelf.state.saving) {
      Js.log("Auto-saving entry with ID: " ++ (ctx.entry |> Entry.id));
      saveChanges(ctx, newSelf.send, appSend);
    };
  },
  initialState: () => {saving: false},
  reducer: (action, _state) =>
    switch (action) {
    | Save => ReasonReact.Update({saving: true})
    },
  render: ({state, send}) =>
    <span className="float-right">
      <button
        className="btn btn-primary btn-sm"
        onClick=(_e => saveChanges(ctx, send, appSend))
        disabled=state.saving>
        (state.saving ? <Icon kind=Icon.Loading /> : <Icon kind=Icon.Save />)
      </button>
    </span>,
};
/* export default class SaveBar extends React.Component {
     constructor(props) {
       super(props);
       this.saveChanges = this.saveChanges.bind(this);
     }

     saveChanges() {
       console.log("Saving changes...");

       // Iterate over all new entries and create / update them on the server.
       for (const entryId in this.props.appState.unsavedEntries) {
         if (this.props.appState.unsavedEntries[entryId].persisted) {
           this.updateEntry(entryId);
         } else {
           this.createEntry(entryId);
         }
       }
     }

     updatedEntry(unsavedEntryId, deletePersisted = true) {
       const unsavedEntryClone = _.cloneDeep(
         this.props.appState.unsavedEntries[unsavedEntryId]
       );

       // Remove the persisted flag - this is doesn't need to be sent to the server.
       if (deletePersisted) {
         delete unsavedEntryClone.persisted;
       }

       return unsavedEntryClone;
     }

     entryTagIds(unsavedEntryId) {
       return _.map(
         this.props.appState.unsavedEntries[unsavedEntryId].tags,
         tag => {
           return tag.id;
         }
       );
     }

     async createMissingTags(unsavedEntryId) {
       console.log(`Checking for unsaved tags in entry with ID ${unsavedEntryId}`);

       for (const tag of this.props.appState.unsavedEntries[unsavedEntryId].tags) {
         if (_.isString(tag.nameHash)) {
           console.log(
             `Found tag with nameHash ${tag.nameHash}. Checking if it has already been created...`
           );

           const savedTag = _.find(this.props.appState.tags, savedTag => {
             return savedTag.nameHash === tag.nameHash;
           });

           if (_.isObject(savedTag)) {
             console.log(
               `Tag with nameHash ${tag.nameHash} seems to have been saved already. Using existing tag...`
             );
             this.updateTagOnUnsavedEntry(unsavedEntryId, savedTag);
           } else {
             console.log(
               `Tag with nameHash ${tag.nameHash} does not exist. Creating it...`
             );
             await this.createTag(unsavedEntryId, tag);
           }
         }
       }
     }

     // For an available saved tag, replace the tag's object in the unsaved entry
     // with an object containing its ID.
     updateTagOnUnsavedEntry(unsavedEntryId, savedTag) {
       const unsavedEntriesClone = _.cloneDeep(this.props.appState.unsavedEntries);

       const oldTagIndex = _.findIndex(
         unsavedEntriesClone[unsavedEntryId].tags,
         oldTag => {
           return oldTag.nameHash === savedTag.nameHash;
         }
       );

       unsavedEntriesClone[oldTagIndex] = { id: savedTag.id };
       this.props.setAppState({ unsavedEntries: unsavedEntriesClone });
     }

     // When a tag is brand new, it'll have to be created, and then added to the
     // master list of tags, and replaced in the unsaved entry referring to it.
     async createTag(unsavedEntryId, unsavedTag) {
       const createTagService = new CreateTagService(
         this.props.appState.token,
         this.props.appState.team.id,
         unsavedTag
       );

       const savedTag = await createTagService.create();

       console.log(
         `Tag with ID ${savedTag.id} has been created. Updating app state...`
       );

       // Clone tags to add newly created tag to the list.
       const tagsClone = _.cloneDeep(this.props.appState.tags);
       tagsClone[savedTag.id] = {
         name: unsavedTag.name,
         nameHash: unsavedTag.nameHash
       };

       // Clone unsaved entries to modify its list of tags.
       const unsavedEntriesClone = _.cloneDeep(this.props.appState.unsavedEntries);
       const unsavedEntryTags = unsavedEntriesClone[unsavedEntryId].tags;

       const oldTagIndex = _.findIndex(unsavedEntryTags, oldTag => {
         return oldTag.nameHash === savedTag.nameHash;
       });

       // Replace tag's full data with just the ID.
       unsavedEntryTags[oldTagIndex] = { id: savedTag.id };

       // Now update both in appState.
       const updatedState = {
         tags: tagsClone,
         unsavedEntries: unsavedEntriesClone
       };

       console.log("Adding new tag by updating appState with: ", updatedState);
       this.props.setAppState(updatedState);
     }

     async updateEntry(entryId) {
       !!! EXTRACTED !!!
     }

     async createEntry(unsavedEntryId) {
       !!! EXTRACTED !!!
     }

     render() {
       return (
         !!! EXTRACTED !!!
       );
     }
   } */