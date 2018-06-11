let str = ReasonReact.stringToElement;

type state = {inProgress: bool};

type action =
  | SignOut;

let component = ReasonReact.reducerComponent("SignOutButton");

module DeleteSessionQuery = [%graphql
  {|
    mutation {
      deleteSession {
        errors
      }
    }
  |}
];

let make = (~appState: Turaku.state, ~appSend, _children) => {
  ...component,
  initialState: () => {inProgress: false},
  reducer: (action, _state) =>
    switch (action) {
    | SignOut =>
      ReasonReact.UpdateWithSideEffects(
        {inProgress: true},
        (
          _self =>
            DeleteSessionQuery.make()
            |> Api.sendQuery(appState.session)
            |> Js.Promise.then_(response => {
                 if (response##deleteSession##errors
                     |> Array.to_list
                     |> List.length == 0) {
                   appSend(Turaku.SignOut);
                 } else {
                   Js.log2(
                     "Some error occured while trying to sign out. Check: ",
                     response,
                   );
                 };
                 Js.Promise.resolve();
               })
            |> ignore
        ),
      )
    },
  render: ({state, send}) =>
    if (state.inProgress) {
      <button
        className="btn btn-secondary btn-sm"
        disabled=(true |> Js.Boolean.to_js_boolean)>
        (str("Signing out..."))
      </button>;
    } else {
      <button
        className="btn btn-secondary btn-sm" onClick=(_e => send(SignOut))>
        (str("Sign Out"))
      </button>;
    },
};