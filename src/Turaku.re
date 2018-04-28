type action =
  | SignedUp
  | SignedIn;

type page =
  | SignUpPage
  | SignInPage
  | DashboardPage(Dashboard.selectable);

type flags = {
  restorationAttempted: bool,
  justSignedUp: bool,
};

type state = {
  session: Session.t,
  currentPage: page,
  flags,
};

let initialState = {
  session: Session.SignedOut,
  currentPage: SignUpPage,
  flags: {
    restorationAttempted: true,
    justSignedUp: false,
  },
};

let reducer = (action, state) =>
  switch (action) {
  | SignedIn =>
    ReasonReact.Update({
      ...state,
      flags: {
        ...state.flags,
        justSignedUp: false,
      },
    })
  | SignedUp =>
    ReasonReact.Update({
      ...state,
      currentPage: SignInPage,
      flags: {
        ...state.flags,
        justSignedUp: true,
      },
    })
  };