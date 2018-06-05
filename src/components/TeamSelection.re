/* import "./teams.css"; */
type state = {
  createFormVisible: bool,
  teamPassword: string,
  teamName: string,
};

type action =
  | ToggleCreateForm
  | UpdateTeamPassword(string)
  | UpdateTeamName(string);

let str = ReasonReact.stringToElement;

let component = ReasonReact.reducerComponent("TeamSelection");

let invitations = (appState: Turaku.state, appSend) =>
  if (appState.invitations |> List.length > 0) {
    <div>
      <h2> (str("Invitations")) </h2>
      <p> (str("You have been invited to join:")) </p>
      (
        appState.invitations
        |> List.map(invitation => <Invitations appState appSend invitation />)
        |> Array.of_list
        |> ReasonReact.arrayToElement
      )
    </div>;
  } else {
    <span />;
  };

let selectTeam = (team, appSend, _event) =>
  /* TODO: See how to select a team with old selectTeam function below. */
  Js.log2("Select this team: ", team);

let teams = (appState: Turaku.state, appSend) =>
  if (appState.teams |> List.length > 0) {
    <div>
      <h2> (str("Your Teams")) </h2>
      <div>
        <ul className="mt-3 teams__ul">
          (
            appState.teams
            |> List.map((team: Team.t) =>
                 <li key=(team |> Team.getId) className="mb-1">
                   <button
                     onClick=(selectTeam(team, appSend))
                     className="btn btn-sm btn-outline-dark">
                     (str(team |> Team.getName))
                   </button>
                 </li>
               )
            |> Array.of_list
            |> ReasonReact.arrayToElement
          )
        </ul>
        <hr />
      </div>
    </div>;
  } else {
    <p> (str("You do not belong to any team, right now. Create one?")) </p>;
  };

let createTeamButton = (state, send) =>
  if (state.createFormVisible) {
    <span />;
  } else {
    <button
      className="btn btn-primary" onClick=(_event => send(ToggleCreateForm))>
      (str("Create a new Team"))
    </button>;
  };

let createTeam = event => {
  event |> DomUtils.preventEventDefault;
  Js.log("Create a team! Whoo!");
};

let updateTeamPassword = (send, _event) => {
  let password = DomUtils.getValueOfInputById("teams__form-password");
  send(UpdateTeamPassword(password));
};

let updateTeamName = (send, _event) => {
  let name = DomUtils.getValueOfInputById("teams__form-name");
  send(UpdateTeamName(name));
};

let createTeamForm = (state, send) =>
  if (state.createFormVisible) {
    <form onSubmit=createTeam>
      <div className="form-group">
        <label htmlFor="teams__form-name"> (str("Name of your team")) </label>
        <input
          className="form-control"
          id="teams__form-name"
          placeholder="Enter your team's name"
          onChange=(updateTeamName(send))
        />
        <small id="teams__form-name-help" className="form-text text-muted">
          (str("You can add team members later."))
        </small>
      </div>
      <div className="form-group">
        <label htmlFor="teams__form-password"> (str("Team Password")) </label>
        <input
          className="form-control"
          id="teams__form-password"
          value=state.teamPassword
          onChange=(updateTeamPassword(send))
        />
        <small id="teams__form-password-help" className="form-text text-muted">
          <ul>
            <li>
              (
                str(
                  "Type random characters into the password field to improve its ",
                )
              )
              <em> (str("randomness")) </em>
              (str("."))
            </li>
            <li>
              (
                str("This is the password used to encode your team's records.")
              )
            </li>
            <li>
              (str("It will be encrypted before being sent to Turaku."))
            </li>
          </ul>
        </small>
      </div>
      <button _type="submit" className="btn btn-primary">
        (str("Create"))
      </button>
      <button
        className="btn btn-secondary ml-2"
        onClick=(_event => send(ToggleCreateForm))>
        (str("Cancel"))
      </button>
    </form>;
  } else {
    <span />;
  };

let make = (~appState, ~appSend, _children) => {
  ...component,
  initialState: () => {
    createFormVisible: false,
    teamPassword: TeamPassword.create() |> TeamPassword.toString,
    teamName: "",
  },
  reducer: (action, state) =>
    switch (action) {
    | ToggleCreateForm =>
      ReasonReact.Update({
        ...state,
        createFormVisible: ! state.createFormVisible,
      })
    | UpdateTeamPassword(password) =>
      /* TODO: Hash the password every time it is updated. See old code below in function updateTeamPassword. */
      ReasonReact.Update({...state, teamPassword: password})
    | UpdateTeamName(name) => ReasonReact.Update({...state, teamName: name})
    },
  render: ({state, send}) =>
    <div className="container">
      <div className="row justify-content-center sign-in__centered-container">
        <div className="col-md-6 align-self-center">
          (invitations(appState, appSend))
          (teams(appState, appSend))
          (createTeamButton(state, send))
          (createTeamForm(state, send))
        </div>
      </div>
    </div>,
};
/* export default class Teams extends React.Component {
     constructor(props) {
       !!! EXTRACTED !!!
     }

     async selectTeam(team) {
       // Decrypt the password and store that now that a team has been selected.
       let password = await new CryptoService(
         this.props.appState.encryptionHash
       ).decrypt(team.encrypted_password);

       let decryptedTeam = _.clone(team);
       delete decryptedTeam.encrypted_password;
       decryptedTeam.password = password;

       this.props.setAppState({ team: decryptedTeam }, () => {
         this.setState({ teamSelected: true }, () => {
           // Store the team in session storage as well, so that when reloading,
           // it can be automatically set as selected team.
           sessionStorage.setItem("team", JSON.stringify(decryptedTeam));
         });
       });
     }

     currentTeams() {
       !!! EXTRACTED !!!
     }

     createTeam(event) {
       event.preventDefault();

       let createService = new CreateService(
         this.props.appState.token,
         this.props.appState.encryptionHash
       );

       let name = document.getElementById("teams__form-name").value;
       let that = this;

       createService
         .create(name, this.state.teamPassword)
         .then(team => {
           console.log("Created team.");

           let updatedTeams = _.cloneDeep(that.props.appState.teams);
           updatedTeams.push(team);

           that.props.setAppState({ teams: updatedTeams }, () => {
             that.setState({ createFormVisible: false });
           });
         })
         .catch(exception => {
           // Handle invalid credentials / exception.
           console.log(exception, "Team creation failed.");
         });
     }

     async updateTeamPassword(event) {
       if (_.isObject(event)) {
         console.log(
           "Hashing " + event.target.value + " to get new team password..."
         );

         const updatedPassword = await new TeamPasswordService().updatePassword(
           event.target.value
         );

         console.log("Updated team password is " + updatedPassword);

         this.setState({ teamPassword: updatedPassword });
       }
     }

     createForm() {
       !!! EXTRACTED !!!
     }

     showCreateForm() {
       this.setState({
         createFormVisible: true,
         teamPassword: new TeamPasswordService().newPassword()
       });
     }

     hideCreateForm() {
       this.setState({ createFormVisible: false });
     }

     incomingInvitations() {
       !!! EXTRACTED !!!
     }

     render() {
       if (!_.isArray(this.props.appState.teams)) {
         return <Redirect to="/sign_in" />;
       }

       if (this.state.teamSelected) {
         return <Redirect to="/dash" />;
       }

       return (
         !!! EXTRACTED !!!
       );
     }
   } */