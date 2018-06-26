let str = ReasonReact.string;

let component = ReasonReact.statelessComponent("Tags");

type ctx = {userData: Turaku.userData};

let navigateToAllEntries = (ctx, appSend, event) => {
  event |> DomUtils.preventMouseEventDefault;
  /* let entryId = Turaku.someEntry(ctx.userData, ctx.dashboardPageData); */
  switch (ctx.userData.dashboardMenu) {
  | EntriesMenu => ()
  | TeamMenu(_) => Turaku.SelectTag(None, ctx.userData) |> appSend
  };
};

let make = (~ctx, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div className="pt-3">
      <div
        onClick=(navigateToAllEntries(ctx, appSend))
        className="dashboard__navlink">
        (str("All tags"))
      </div>
      <div className="dashboard__navlink"> ("Tag 1" |> str) </div>
      <div className="dashboard__navlink"> ("Tag 2" |> str) </div>
      <div className="dashboard__navlink"> ("Tag 3" |> str) </div>
      <div className="dashboard__navlink"> ("..." |> str) </div>
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