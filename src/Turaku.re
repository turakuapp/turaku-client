type action =
  | SignedUp
  | SignedIn;

type page =
  | SignUpPage
  | SignInPage
  | DashboardPage(Dashboard.selectable);

type state = {
  restorationAttempted: bool,
  session: Session.t,
  currentPage: page
};

let initialState = {
  restorationAttempted: true,
  session: Session.SignedOut,
  currentPage: SignUpPage
};