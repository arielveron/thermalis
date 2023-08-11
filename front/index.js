const historySize = 20;
const smoothSize = 3;

document
  .getElementById("configBackend")
  .addEventListener("click", configBackendAddress);
// document.getElementById("dispTempMin").addEventListener("click", setTempMin);
// document.getElementById("dispTempMax").addEventListener("click", setTempMax);

let historicTemps = [];
storage = window.localStorage;
let backendAddress = getConfig();

if (!backendAddress) {
  backendAddress = configBackendAddress();
}

const index = setInterval(function () {
  fetch(parseGet(backendAddress))
    .then((response) => response.json())
    .then((json) => {
      const {
        currentTemp,
        motorOn,
        tempMax,
        tempMin,
        backendVersion,
        monitoringMode,
        timeSpanName,
        motorStartTime,
        motorEndTime,
        timeSpanId,
        tsForced
      } = json;

      const workingTemp = displayAverageTemp(currentTemp, historicTemps);

      displayBackendVersion(backendVersion);
      displayActiveTimeSpan(timeSpanName, tsForced);
      displayMonitorModeLogo(monitoringMode);
      displayProgressBarTemp(workingTemp, tempMin, tempMax);
      displayEngineMessage(motorStartTime, motorEndTime, motorOn);
      switchEngineDisplay(motorOn);
      displayMinMaxTemps(workingTemp, tempMin, tempMax);
      enableForceEngine(tsForced);
    })
    .catch((err) => console.log("Solicitud fallida", err));
}, 500);
