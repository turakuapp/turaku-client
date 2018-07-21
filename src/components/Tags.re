let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("Tags");

type ctx = {
  userData: Turaku.userData,
  team: Team.t,
};

let navigateToAllEntries = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  /* let entryId = Turaku.someEntry(ctx.userData, ctx.dashboardPageData); */
  switch (ctx.userData.dashboardMenu) {
  | EntriesMenu => ()
  | TeamMenu(_) => Turaku.SelectTag(None) |> appSend
  };
};

let tagLinks = (ctx, appSend) =>
  ctx.team
  |> Team.tags
  |> SelectableList.all
  |> List.map(tag =>
       <div
         className="cursor-pointer p-2 pl-6 font-thin hover:bg-grey-light"
         key=(tag |> Tag.id)>
         (tag |> Tag.name |> str)
       </div>
     )
  |> Array.of_list
  |> ReasonReact.array;

let make = (~ctx, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div className="mt-2">
      <div
        onClick=(navigateToAllEntries(ctx, appSend))
        className="cursor-pointer p-2 pl-4 hover:bg-grey-light">
        ("All tags" |> str)
      </div>
      (tagLinks(ctx, appSend))
    </div>,
};
/* export default class Tags extends React.Component {
     componentDidMount() {
       const listService = new ListTagsService(
         this.props.appState.token,
         this.props.appState.team.id
       );

       listService.list().then(encryptedTags => {
         for (const encryptedTag of encryptedTags) {
           this.decryptAndStoreTag(encryptedTag);
         }
       });
     }

     async decryptAndStoreTag(tag) {
       const cryptoService = new CryptoService(
         this.props.appState.team.password,
         true
       );

       const name = await cryptoService.decrypt(tag.encryptedName);
       const tagsClone = _.cloneDeep(this.props.appState.tags);

       tagsClone[tag.id] = { name: name, nameHash: tag.nameHash };

       this.props.setAppState({ tags: tagsClone });
     }

     savedTags() {
       if (_.isEmpty(this.props.appState.tags)) {
         return;
       }

       return _.map(this.props.appState.tags, tag => {
         return (
           <div>
             <a href="#">{tag.name}</a>
           </div>
         );
       });
     }

     render() {
       return (
         <div>
           <h3>Tags</h3>

           <div>
             <Link to="/dash/entries/all">All tags</Link>
             {this.savedTags()}
           </div>
         </div>
       );
     }
   } */