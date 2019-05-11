let str = React.string;

module DeleteSessionQuery = [%graphql
  {|
    mutation {
      deleteSession {
        errors
      }
    }
  |}
];

let signOut = (session, setInProgress, appSend, _event) => {
  setInProgress(_ => true);

  DeleteSessionQuery.make()
  |> Api.sendAuthenticatedQuery(session)
  |> Js.Promise.then_(response => {
       if (response##deleteSession##errors |> Array.to_list |> List.length == 0) {
         appSend(Turaku.SignOut(session));
       } else {
         Js.log2(
           "Some error occured while trying to sign out. Check: ",
           response,
         );
       };
       setInProgress(_ => false);
       Js.Promise.resolve();
     })
  |> ignore;
};

[@react.component]
let make = (~session, ~appSend) => {
  let (inProgress, setInProgress) = React.useState(() => false);

  if (inProgress) {
    <div className="cursor-wait p-2 pl-4 text-grey-dark">
      {"Signing out..." |> str}
    </div>;
  } else {
    <div
      className="cursor-pointer p-2 pl-4 hover:bg-grey-lighter"
      onClick={signOut(session, setInProgress, appSend)}>
      {"Sign Out" |> str}
    </div>;
  };
};