let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("Users");

let make = (~appState, ~appSend, _children) => {
  ...component,
  render: (_self) => <div>(str("Users"))</div>
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
      <form onSubmit={this.inviteUser}>
        <div className="form-group">
          <label htmlFor="users__invite-form-email">Email Address</label>
          <input
            className="form-control"
            id="users__invite-form-email"
            placeholder="Enter your team member's email address"
            type="email"
            required={true}
          />
        </div>

        <button type="submit" className="btn btn-primary">
          Invite
        </button>

        <button
          className="btn btn-secondary ml-2"
          onClick={this.hideInviteForm}
        >
          Cancel
        </button>
      </form>
    );
  }

  render() {
    return (
      <div>
        {this.listOfUsers()}
        {this.listOfInvitedUsers()}

        {!this.state.inviteFormVisible && (
          <button className="btn btn-primary" onClick={this.showInviteForm}>
            Invite a team member
          </button>
        )}

        {this.state.inviteFormVisible && this.inviteForm()}
      </div>
    );
  }
} */