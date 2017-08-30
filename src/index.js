import React from "react";
import ReactDOM from "react-dom";
import "bootstrap/dist/css/bootstrap.css";
import "./assets/style/index.css";
import AppState from "./components/appState";
import App from "./components/app.js";
import registerServiceWorker from "./registerServiceWorker";

ReactDOM.render(
  <AppState>
    <App />
  </AppState>,
  document.getElementById("root")
);

registerServiceWorker();
