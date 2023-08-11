storage = window.localStorage;
let backendAddress = getConfig();

if (!backendAddress) {
  backendAddress = configBackendAddress();
}

const timeSpans = document.getElementById("timespans");
let idEnabled;
let backendTime;

const config = setInterval(function () {
  fetch(parseGet(backendAddress))
    .then((response) => response.json())
    .then((json) => {
      idEnabled = json.timeSpanid;
      tsForced = json.tsForced;
    });

  fetch(parseTimeSpans(backendAddress))
    .then((response) => response.json())
    .then((json) => {
      displayBackendTime(json.time);
      let ts = json.timeSpans;
      let output = "";
      for (let i = ts.length - 1; i >= 0; i--) {
        let selection = "";
        if (i == idEnabled) {
          if (tsForced >= 0) {
            selection = "tsForced";
          } else {
            selection = "tsSelected";
          }
        }
        output += `
        <div class="timeSpan ${selection}">
        <div>${ts[i].id}</div>
        <div class="name">${ts[i].tsName}</div>
        <div class="starttime">${ts[i].startTime}</div>
        <div class="endtime">${ts[i].endTime}</div>
        <div class="tempmin">${ts[i].tempMin}</div>
        <div class="tempmax">${ts[i].tempMax}</div>
        <div class="enabled">${ts[i].enabled}</div>
        <div class="force"><span>${ts[i].tempMin} ~ ${ts[i].tempMax}</span></div>
        </div>
          `;
      }
      // console.log(output)
      timeSpans.innerHTML = output;

      const parameters = ["name", "starttime", "endtime", "tempmin", "tempmax", "enabled"];
      parameters.forEach(param => enableModifyParameter(param));
      enableForceButton(tsForced);
    })
    .catch((err) => console.log("Solicitud fallida", err));
}, 1000);
