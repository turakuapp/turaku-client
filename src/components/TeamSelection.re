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
               key=(invitation |> InvitationFromTeam.id)
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

let selectTeam = (appSend, team, _event) => {
  /* TODO: See how to select a team with old selectTeam function below. */
  Js.log2("Selecting Team with ID: ", team);
  appSend(Turaku.SelectTeam(team));
};

let teams = (ctx, appSend) => {
  let allTeams = ctx.userData.teams |> SelectableList.all;

  if (allTeams |> List.length > 0) {
    <div>
      <h2> (str("Your Teams")) </h2>
      <div>
        <ul className="mt-3 list-reset">
          (
            allTeams
            |> List.map((team: Team.t) =>
                 <li key=(team |> Team.id) className="mt-2">
                   <button
                     onClick=(selectTeam(appSend, team))
                     className="btn btn-blue">
                     (str(team |> Team.name))
                   </button>
                 </li>
               )
            |> Array.of_list
            |> ReasonReact.array
          )
        </ul>
      </div>
    </div>;
  } else {
    <p> (str("You do not belong to any team, right now. Create one?")) </p>;
  };
};

let createTeamButton = send =>
  <button
    className="btn border hover:bg-grey-light mt-2"
    onClick=(_event => send(ToggleCreateForm))>
    (str("Create a new Team"))
  </button>;

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
           encryptedData |> EncryptedData.iv |> InitializationVector.toString,
         ~ciphertext=
           encryptedData |> EncryptedData.ciphertext |> CipherText.toString,
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
         appSend(Turaku.AddTeam(team));
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
  <form onSubmit=(createTeam(ctx, appSend, state))>
    <div>
      <label htmlFor="teams__form-name"> ("Name of your team" |> str) </label>
      <input
        autoFocus=true
        required=true
        className="rounded bg-grey-light focus:bg-grey-lighter p-2 mt-2 w-full"
        id="teams__form-name"
        onChange=(updateTeamName(send))
      />
      <small className="block text-grey-dark mt-1">
        ("You can add team members later." |> str)
      </small>
    </div>
    <div className="mt-3">
      <label htmlFor="teams__form-password"> ("Team password" |> str) </label>
      <input
        required=true
        className="rounded bg-grey-light focus:bg-grey-lighter p-2 mt-2 w-full"
        id="teams__form-password"
        value=(state.teamPassword |> TeamPassword.toString)
        onChange=(updateTeamPassword(send))
      />
      <ul className="pl-8 mt-2">
        <li>
          <small className="block text-grey-dark">
            (
              "Type random characters into the password field to improve its "
              |> str
            )
            <em> ("randomness" |> str) </em>
            ("." |> str)
          </small>
        </li>
        <li>
          <small className="block text-grey-dark mt-1">
            ("This is the password used to encode your team's records." |> str)
          </small>
        </li>
        <li>
          <small className="block text-grey-dark mt-1">
            ("It will be encrypted before being sent to Turaku." |> str)
          </small>
        </li>
      </ul>
    </div>
    <button
      type_="submit"
      className="btn mt-5 bg-green hover:bg-green-dark text-white">
      (str("Create"))
    </button>
    <button
      className="btn mt-5 ml-2 border hover:bg-grey-light cursor-pointer"
      onClick=(_event => send(ToggleCreateForm))>
      ("Cancel" |> str)
    </button>
  </form>;

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
        createFormVisible: !state.createFormVisible,
      })
    | UpdateTeamPassword(password) =>
      /* TODO: Hash the password every time it is updated. See old code below in function updateTeamPassword. */
      ReasonReact.Update({...state, teamPassword: password})
    | UpdateTeamName(name) => ReasonReact.Update({...state, teamName: name})
    },
  render: ({state, send}) =>
    <div className="container mx-auto px-4">
      <div className="flex justify-center h-screen">
        <div className="w-full md:w-1/2 self-auto md:self-center pt-4 md:pt-0">
          (invitations(ctx, appSend))
          (
            state.createFormVisible ?
              createTeamForm(ctx, appSend, state, send) :
              [|teams(ctx, appSend), createTeamButton(send)|]
              |> ReasonReact.array
          )
        </div>
      </div>
    </div>,
};
