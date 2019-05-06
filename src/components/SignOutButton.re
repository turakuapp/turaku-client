let str = ReasonReact.string;

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

let make = (~session, ~appSend, _children) => {
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
            |> Api.sendAuthenticatedQuery(session)
            |> Js.Promise.then_(response => {
                 if (response##deleteSession##errors
                     |> Array.to_list
                     |> List.length == 0) {
                   appSend(Turaku.SignOut(session));
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
      <div className="cursor-wait p-2 pl-4 text-grey-dark">
        {"Signing out..." |> str}
      </div>;
    } else {
      <div
        className="cursor-pointer p-2 pl-4 hover:bg-grey-lighter"
        onClick={_e => send(SignOut)}>
        {"Sign Out" |> str}
      </div>;
    },
};