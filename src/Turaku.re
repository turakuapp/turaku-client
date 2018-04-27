type action =
  | AttemptRestoration
  | SignedUp
  | SignedIn;

type page =
  | SignUpPage
  | SignInPage
  | DashboardPage(Dashboard.selectable);

type state = {
  session: Session.t,
  currentFocus: page
};