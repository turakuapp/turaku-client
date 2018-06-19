let str = ReasonReact.string;

type state = {inProgress: bool};

type action =
  | SignOut;

type ctx = {
  userData: Turaku.userData,
  dashboardPageData: Turaku.dashboardPageData,
};

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

let make = (~ctx, ~appSend, _children) => {
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
            |> Api.sendAuthenticatedQuery(ctx.userData.session)
            |> Js.Promise.then_(response => {
                 if (response##deleteSession##errors
                     |> Array.to_list
                     |> List.length == 0) {
                   appSend(Turaku.SignOut(ctx.userData.session));
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
      <button className="btn btn-secondary btn-sm" disabled=true>
        (str("Signing out..."))
      </button>;
    } else {
      <button
        className="btn btn-secondary btn-sm" onClick=(_e => send(SignOut))>
        (str("Sign Out"))
      </button>;
    },
};