let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("Tags");

type bag = {
  userData: Turaku.userData,
  dashboardPageData: Turaku.dashboardPageData,
};

let make = (~bag: bag, ~appSend, _children) => {
  ...component,
  render: _self =>
    <div className="dashboard__navlink"> (str("All tags")) </div>,
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