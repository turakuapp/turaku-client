[%bs.raw {|require("./saveBar.css")|}];
let str = ReasonReact.string;

type ctx = {
  userData: Turaku.userData,
  team: Team.t,
};

let component = ReasonReact.statelessComponent("SaveBar");

let saveChanges = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  Js.log("TODO: Save changes");
};

let make = (~ctx, ~appSend, _children) => {
  ...component,
  render: _self =>
    if (ctx.team
        |> Team.entries
        |> SelectableList.all
        |> List.filter(Entry.unpersisted)
        |> List.length > 0) {
      <div className="fixed-bottom text-center p-2 save-bar">
        <button
          className="btn btn-outline-light btn-sm"
          onClick=(saveChanges(ctx, appSend))>
          (str("Save all changes"))
        </button>
      </div>;
    } else {
      ReasonReact.null;
    },
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
       console.log(`Updating existing entry with ID ${entryId}`);

       await this.createMissingTags(entryId);

       const updateService = new UpdateEntryService(
         this.props.appState.token,
         this.props.appState.team.password,
         entryId,
         this.updatedEntry(entryId),
         this.entryTagIds(entryId)
       );

       const updatedEntryId = await updateService.execute();

       console.log(
         `Entry with ID ${entryId} has been updated. Updating app state...`
       );

       const entriesClone = _.cloneDeep(this.props.appState.entries);
       const unsavedEntriesClone = _.cloneDeep(this.props.appState.unsavedEntries);
       entriesClone[updatedEntryId] = this.updatedEntry(entryId, false);
       delete unsavedEntriesClone[entryId];

       const newAppState = {
         entries: entriesClone,
         unsavedEntries: unsavedEntriesClone
       };

       console.log("Updating appState with data: ", newAppState);

       this.props.setAppState(newAppState, () => {
         console.log("New app state should include: ", newAppState);
       });
     }

     async createEntry(unsavedEntryId) {
       console.log("Creating new entry with temp ID: " + unsavedEntryId);

       await this.createMissingTags(unsavedEntryId);

       const createService = new CreateEntryService(
         this.props.appState.token,
         this.props.appState.team.password,
         this.props.appState.team.id,
         this.updatedEntry(unsavedEntryId),
         this.entryTagIds(unsavedEntryId)
       );

       const savedEntryId = await createService.execute();

       console.log(
         "New entry with temp ID " +
         unsavedEntryId +
         " now has permanent ID " +
         savedEntryId +
         ". Updating..."
       );

       // TODO: Shouldn't this step be setting the persisted flag for the newly created entry?

       const entriesClone = _.cloneDeep(this.props.appState.entries);
       const unsavedEntriesClone = _.cloneDeep(this.props.appState.unsavedEntries);
       entriesClone[savedEntryId] = this.updatedEntry(unsavedEntryId);
       delete unsavedEntriesClone[unsavedEntryId];

       const newAppState = {
         entries: entriesClone,
         unsavedEntries: unsavedEntriesClone
       };

       if (this.props.appState.entryId === unsavedEntryId) {
         newAppState.entryId = "" + savedEntryId;
       }

       console.log("Updating appState with data: ", newAppState);

       this.props.setAppState(newAppState, () => {
         console.log("New app state should include: ", newAppState);
       });
     }

     render() {
       return (
         !!! EXTRACTED !!!
       );
     }
   } */