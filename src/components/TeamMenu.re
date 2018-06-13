let str = ReasonReact.stringToElement;

type state = {inviteFormVisible: bool};

type action =
  | ToggleInviteForm;

type bag = {
  userData: Turaku.userData,
  dashboardPageData: Turaku.dashboardPageData,
};

let component = ReasonReact.reducerComponent("TeamMenu");

let toggleInviteForm = (send, event) => {
  event |> DomUtils.preventMouseEventDefault;
  send(ToggleInviteForm);
};

let invitationToggle = (state, send) =>
  if (state.inviteFormVisible) {
    ReasonReact.nullElement;
  } else {
    <button
      className="btn btn-primary btn-sm" onClick=(toggleInviteForm(send))>
      ("Invite" |> str)
    </button>;
  };

let teamMembers = (bag, appSend) => {
  let teamMembers =
    Turaku.currentTeam(bag.userData, bag.dashboardPageData)
    |> Team.teamMembers;
  if (teamMembers |> List.length > 0) {
    <div>
      <ul>
        (
          teamMembers
          |> List.map(teamMember =>
               <li key=(teamMember |> TeamMember.id)>
                 (teamMember |> TeamMember.name |> str)
                 (" - " |> str)
                 <code>
                   (teamMember |> TeamMember.email |> Email.toString |> str)
                 </code>
               </li>
             )
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </ul>
    </div>;
  } else {
    <div> ("Loading users..." |> str) </div>;
  };
};

let invitedMembers = (bag, appSend) => ReasonReact.nullElement;

let inviteUser = (bag, appSend, event) => {
  Js.log("Invite a new user!");
  event |> DomUtils.preventEventDefault;
};

let invitationForm = (bag, appSend, state, send) =>
  if (state.inviteFormVisible) {
    <form onSubmit=(inviteUser(bag, appSend))>
      <div className="form-group">
        <label htmlFor="users__invite-form-email">
          ("Email Address" |> str)
        </label>
        <input
          className="form-control"
          id="users__invite-form-email"
          placeholder="Enter your team member's email address"
          _type="email"
          required=(true |> Js.Boolean.to_js_boolean)
        />
      </div>
      <button _type="submit" className="btn btn-primary">
        ("Invite" |> str)
      </button>
      <button
        className="btn btn-secondary ml-2" onClick=(toggleInviteForm(send))>
        ("Cancel" |> str)
      </button>
    </form>;
  } else {
    ReasonReact.nullElement;
  };

module UsersQuery = [%graphql
  {|
  query($teamId: ID!) {
    team(id: $teamId) {
      users {
        id
        name
        email
      }
    }
  }
  |}
];

let refreshUsers = (bag, appSend) => {
  let teamId =
    Turaku.currentTeam(bag.userData, bag.dashboardPageData) |> Team.id;
  UsersQuery.make(~teamId, ())
  |> Api.sendAuthenticatedQuery(bag.userData.session)
  |> Js.Promise.then_(response => {
       let rawUsers = response##team##users;
       Js.log(
         "Loaded users! Count: " ++ (rawUsers |> Array.length |> string_of_int),
       );
       let teamMembers =
         rawUsers
         |> Array.map(rawUser =>
              TeamMember.create(
                rawUser##id,
                rawUser##name,
                rawUser##email |> Email.create,
              )
            )
         |> Array.to_list;
       appSend(Turaku.RefreshTeamMembers(teamId, teamMembers, bag.userData));
       Js.Promise.resolve();
     })
  |> ignore;
};

let make = (~bag, ~appSend, _children) => {
  ...component,
  didMount: _self => {
    refreshUsers(bag, appSend);
    ReasonReact.NoUpdate;
  },
  initialState: () => {inviteFormVisible: false},
  reducer: (action, state) =>
    switch (action) {
    | ToggleInviteForm =>
      ReasonReact.Update({inviteFormVisible: ! state.inviteFormVisible})
    },
  render: ({state, send}) =>
    <div className="row">

        <div className="col-3">
          <div className="team-menu__members">
            <div className="pt-2">
              <input _type="text" placeholder="Search" className="mr-2" />
              (invitationToggle(state, send))
            </div>
            (invitationForm(bag, appSend, state, send))
            (invitedMembers(bag, appSend))
            (teamMembers(bag, appSend))
          </div>
        </div>
        <div className="col entry-editor__container">
          ("Permissions go here" |> str)
        </div>
      </div>,
      /* (bag.entryMenuData.entryId |> getSelection(bag, appSend)) */
};
/* export default class Users extends React.Component {
     constructor(props) {
       super(props);

       this.state = { inviteFormVisible: false };

       this.inviteUser = this.inviteUser.bind(this);
       this.hideInviteForm = this.hideInviteForm.bind(this);
       this.showInviteForm = this.showInviteForm.bind(this);
     }

     componentDidMount() {
       let service = new ListUsersService(
         this.props.appState.team,
         this.props.appState.token
       );

       service
         .list()
         .then(list => {
           // TODO: Sync users instead of over-writing if already loaded.
           this.props.setAppState({
             users: list.users,
             invitedUsers: list.invitedUsers
           });
         })
         .catch(exception => {
           // TODO: Handle load error here.
           console.log(exception, "List users failed. What do we do? :-(");
         });
     }

     listOfUsers() {
       if (_.isArray(this.props.appState.users)) {
         return (
           <div>
             <h2>Team Members</h2>

             <ul>
               {_.map(this.props.appState.users, user => {
                 return (
                   <li key={user.id}>
                     {user.name} <code>{user.email}</code>
                   </li>
                 );
               })}
             </ul>
           </div>
         );
       } else {
         return <span>Loading users...</span>;
       }
     }

     listOfInvitedUsers() {
       if (_.isArray(this.props.appState.invitedUsers)) {
         return (
           <div>
             <h2>Invited Users</h2>

             <ul>
               {_.map(this.props.appState.invitedUsers, invitedUser => {
                 return (
                   <li key={invitedUser.id}>
                     <code>{invitedUser.email}</code>
                   </li>
                 );
               })}
             </ul>
           </div>
         );
       } else {
         return <span>Loading invited users...</span>;
       }
     }

     inviteUser(event) {
       event.preventDefault();
       let that = this;
       let email = document.getElementById("users__invite-form-email").value;
       let inviteService = new CreateInvitationService(this.props.appState.token);

       console.log("Inviting user with email " + email + " to team...");

       inviteService
         .invite(email, this.props.appState.team.id)
         .then(user => {
           console.log("Invited user.");

           let updatedInvitedUsers = _.cloneDeep(that.props.appState.invitedUsers);
           updatedInvitedUsers.push(user);

           that.props.setAppState({ invitedUsers: updatedInvitedUsers }, () => {
             that.setState({ inviteFormVisible: false });
           });
         })
         .catch(exception => {
           // Handle all the situations where an error is expected.
           console.log(exception, "User invite failed.");
         });
     }

     hideInviteForm() {
       this.setState({ inviteFormVisible: false });
     }

     showInviteForm() {
       this.setState({ inviteFormVisible: true });
     }

     inviteForm() {
       return (
         !!! EXTRACTED !!!
       );
     }

     render() {
       return (
         !!! EXTRACTED !!!
       );
     }
   } */