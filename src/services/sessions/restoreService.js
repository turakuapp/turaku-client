import ApiService from "../apiService";
import _ from "lodash";

export default class RestoreService {
  constructor(setAppState) {
    this.setAppState = setAppState;
  }

  canRestore() {
    return _.isString(sessionStorage.getItem("token"));
  }

  restore() {
    return new Promise((resolve, reject) => {
      if (_.isString(sessionStorage.getItem("token"))) {
        let token = sessionStorage.getItem("token");

        // Try to fetch the encryption salt and teams using token.
        let api = new ApiService(token);
        let that = this;

        resolve(
          api
            .get("sessions/restore")
            .then(response => {
              let sessionData = {
                token: token,
                encryptionSalt: response.user.encryption_salt,
                teams: response.teams,
                incomingInvitations: response.incoming_invitations
              };

              if (_.isObject(that.selectedTeam())) {
                sessionData["team"] = that.selectedTeam();
              }

              // Store particulars of the response in application state. Note that
              // the server only has a hashed token so nothing is returned, unlike
              // when signing in. Instead, the locally sourced value must be used
              // to update state.
              that.setAppState(sessionData);

              return Promise.resolve(true);
            })
            .catch(response => {
              console.log(response, "POST sessions -> failure");

              if (response.exception === "TokenAuthenticationFailedException") {
                that.deleteStoredSessionData();
              } else {
                return Promise.reject(
                  new Error("Response from API indicated an unknown failure.")
                );
              }
            })
        );
      } else {
        reject("Token is not present in session storage.");
      }
    });
  }

  deleteStoredSessionData() {
    sessionStorage.clear();
  }

  selectedTeam() {
    let team = sessionStorage.getItem("team");

    if (_.isString(team)) {
      return JSON.parse(team);
    }
  }
}
