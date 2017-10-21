import ApiService from "../apiService";
import _ from "lodash";

export default class RestoreService {
  constructor(setAppState) {
    this.setAppState = setAppState;
    this.sessionToken = sessionStorage.getItem("token");
    this.sessionEncryptionHash = sessionStorage.getItem("encryptionHash");
  }

  canRestore() {
    return (
      _.isString(this.sessionToken) && _.isString(this.sessionEncryptionHash)
    );
  }

  restore() {
    return new Promise((resolve, reject) => {
      if (this.canRestore()) {
        // Try to fetch teams using token.
        let api = new ApiService(this.sessionToken);
        let that = this;

        resolve(
          api
            .get("sessions/restore")
            .then(response => {
              let sessionData = {
                token: this.sessionToken,
                encryptionHash: this.sessionEncryptionHash,
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
