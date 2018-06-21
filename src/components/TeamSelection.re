[%bs.raw {|require("./teamSelection.css")|}];

type state = {
  createFormVisible: bool,
  teamPassword: TeamPassword.t,
  teamName: string,
};

type action =
  | ToggleCreateForm
  | UpdateTeamPassword(TeamPassword.t)
  | UpdateTeamName(string);

let str = ReasonReact.string;

type ctx = {userData: Turaku.userData};

let component = ReasonReact.reducerComponent("TeamSelection");

let invitations = (ctx, appSend) =>
  if (ctx.userData.invitations |> List.length > 0) {
    <div>
      <h2> (str("Invitations")) </h2>
      <p> (str("You have been invited to join:")) </p>
      (
        ctx.userData.invitations
        |> List.map(invitation =>
             <IncomingInvitation
               ctx={userData: ctx.userData, invitation}
               appSend
             />
           )
        |> Array.of_list
        |> ReasonReact.array
      )
    </div>;
  } else {
    ReasonReact.null;
  };

let selectTeam = (ctx, appSend, team, _event) => {
  /* TODO: See how to select a team with old selectTeam function below. */
  Js.log2("Selecting Team with ID: ", team);
  appSend(Turaku.SelectTeam(team, ctx.userData));
};

let teams = (ctx, appSend) => {
  let allTeams = ctx.userData.teams |> SelectableList.all;

  if (allTeams |> List.length > 0) {
    <div>
      <h2> (str("Your Teams")) </h2>
      <div>
        <ul className="mt-3 teams__ul">
          (
            allTeams
            |> List.map((team: Team.t) =>
                 <li key=(team |> Team.id) className="mb-1">
                   <button
                     onClick=(selectTeam(ctx, appSend, team))
                     className="btn btn-sm btn-outline-dark">
                     (str(team |> Team.name))
                   </button>
                 </li>
               )
            |> Array.of_list
            |> ReasonReact.array
          )
        </ul>
        <hr />
      </div>
    </div>;
  } else {
    <p> (str("You do not belong to any team, right now. Create one?")) </p>;
  };
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

module CreateTeamQuery = [%graphql
  {|
  mutation($name: String! $iv: String!, $ciphertext: String!) {
    createTeam(name: $name, encryptedPassword: {iv: $iv, ciphertext: $ciphertext}) {
      team {
        id
      }
      errors
    }
  }
  |}
];

let createTeam = (ctx, appSend, state, event) => {
  event |> DomUtils.preventEventDefault;
  Js.log(
    "Creating a team with name "
    ++ state.teamName
    ++ " and password (B64) "
    ++ (state.teamPassword |> TeamPassword.toString),
  );
  let encryptionKey = ctx.userData.session |> Session.getCryptographicKey;
  EncryptedData.encrypt(
    encryptionKey,
    state.teamPassword |> TeamPassword.toString,
  )
  |> Js.Promise.then_(encryptedData =>
       CreateTeamQuery.make(
         ~name=state.teamName,
         ~iv=
           encryptedData
           |> EncryptedData.getIV
           |> EncryptedData.InitializationVector.toString,
         ~ciphertext=
           encryptedData
           |> EncryptedData.getCiphertext
           |> EncryptedData.CipherText.toString,
         (),
       )
       |> Api.sendAuthenticatedQuery(ctx.userData.session)
     )
  |> Js.Promise.then_(response => {
       let team = response##createTeam##team;
       switch (team) {
       | None =>
         Js.log2(
           "Something went wrong when trying to create a team. Errors: ",
           response##createTeam##errors |> Js.Array.joinWith(", "),
         )
       | Some(t) =>
         let team = Team.create(t##id, state.teamName, state.teamPassword);
         appSend(Turaku.CreateTeam(team, ctx.userData));
       };
       Js.Promise.resolve();
     })
  |> ignore;
};

let updateTeamPassword = (send, _event) =>
  DomUtils.getValueOfInputById("teams__form-password")
  |> TeamPassword.recreate
  |> Js.Promise.then_(password => {
       send(UpdateTeamPassword(password));
       Js.Promise.resolve();
     })
  |> ignore;

let updateTeamName = (send, _event) => {
  let name = DomUtils.getValueOfInputById("teams__form-name");
  send(UpdateTeamName(name));
};

let createTeamForm = (ctx, appSend, state, send) =>
  if (state.createFormVisible) {
    <form onSubmit=(createTeam(ctx, appSend, state))>
      <div className="form-group">
        <label htmlFor="teams__form-name"> (str("Name of your team")) </label>
        <input
          required=true
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
          required=true
          className="form-control"
          id="teams__form-password"
          value=(state.teamPassword |> TeamPassword.toString)
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

let make = (~ctx, ~appSend, _children) => {
  ...component,
  initialState: () => {
    createFormVisible: false,
    teamPassword: TeamPassword.create(),
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
          (invitations(ctx, appSend))
          (teams(ctx, appSend))
          (createTeamButton(state, send))
          (createTeamForm(ctx, appSend, state, send))
        </div>
      </div>
    </div>,
};