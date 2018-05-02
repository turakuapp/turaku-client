let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("EntryField");

let make = (~appState, ~appSend, ~field, _children) => {
  ...component,
  render: _self => <div> (str("Field")) </div>,
};
/* export default class Field extends React.Component {
     constructor(props) {
       super(props);

       this.state = { passwordVisible: false };

       this.handleChange = this.handleChange.bind(this);
       this.toggleVisibility = this.toggleVisibility.bind(this);
     }

     handleChange(event) {
       console.log("Saving change to a text field...");

       this.saveChangeToField(event.target.value);
     }

     saveChangeToField(newValue) {
       const entryId = this.props.appState.entryId;
       const unsavedEntries = _.cloneDeep(this.props.appState.unsavedEntries);
       let unsavedEntry = unsavedEntries[entryId];

       if (!_.isObject(unsavedEntry)) {
         // The unsaved entry doesn't exist, copy it into the clone
         // of unsaved entries list from list of saved entries.
         unsavedEntry = _.cloneDeep(this.props.appState.entries[entryId]);
         unsavedEntries[entryId] = unsavedEntry;
       }

       const fieldIndex = _.findIndex(unsavedEntry.fields, [
         "name",
         this.props.field.name
       ]);

       const updatedField = _.cloneDeep(this.props.field);
       updatedField.value = newValue;
       unsavedEntry.fields[fieldIndex] = updatedField;

       console.log("Setting updated unsavedEntries: ", unsavedEntries);
       this.props.setAppState({ unsavedEntries: unsavedEntries });
     }

     isPassword() {
       return this.props.field.type === "password";
     }

     toggleVisibility() {
       this.setState({ passwordVisible: !this.state.passwordVisible });
     }

     fieldType() {
       if (this.props.field.type !== "password" || this.state.passwordVisible) {
         return "text";
       } else {
         return "password";
       }
     }

     render() {
       return (
         <div className="row">
           <div className="col-2">
             {/* TODO: Clicking the field name should copy the contents */}
             <button>{this.props.field.name}</button>
           </div>
           <div className="col">
             <input
               type={this.fieldType()}
               value={this.props.field.value}
               onChange={this.handleChange}
             />

             {this.isPassword() && (
               <button
                 className="ml-2 btn btn-sm btn-outline-secondary"
                 onClick={this.toggleVisibility}
               >
                 {this.state.passwordVisible && <EyeOffIcon />}
                 {!this.state.passwordVisible && <EyeIcon />}
               </button>
             )}
           </div>
         </div>
       );
     }
   } */