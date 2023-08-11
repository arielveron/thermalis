function displayMonitorModeLogo(monitoringMode) {
  if (monitoringMode == "HelaNASA") {
    document.getElementById("helaNasaLogo").classList?.remove("hide");
    document.getElementById("termoNasaLogo").classList?.add("hide");
  }
  if (monitoringMode == "TermoNASA") {
    document.getElementById("helaNasaLogo").classList?.add("hide");
    document.getElementById("termoNasaLogo").classList?.remove("hide");
  }
}

function displayBackendVersion(backendVersion) {
  const displayBackendVersion = document.getElementById("dispBackendVersion");
  displayBackendVersion.innerHTML = backendVersion;
}

function displayActiveTimeSpan(timeSpanName, tsForced) {
  const displayActiveTimeSpan = document.getElementById("dispActiveTimeSpan");
  const displayActiveTimeForced = document.getElementById(
    "dispActiveTimeForced"
  );
  let displayForced = "";
  if (tsForced >= 0) displayForced = " (forced)";

  displayActiveTimeSpan.innerHTML = timeSpanName;
  displayActiveTimeForced.innerHTML = displayForced;
}

function displayBackendTime(backendTime) {
  const displayBackendTime = document.getElementById("displayBackendTime");
  displayBackendTime.innerHTML = backendTime;
}

function displayAverageTemp(newTemp, historicTemps) {
  const temperature = document.getElementById("temperature");
  let result;
  enqueueLIFO(historicTemps, newTemp);
  const lastTemps = historicTemps.slice(-smoothSize);
  const avgTemp = lastTemps.reduce((a, b) => a + b, 0) / lastTemps.length;
  result = avgTemp.toFixed(1);

  temperature.innerHTML = result;
  return result;
}

function enqueueLIFO(historicTemps, newTemp) {
  if (historicTemps.length < historySize) {
    historicTemps.push(newTemp);
  } else {
    historicTemps.shift();
    historicTemps.push(newTemp);
  }
}

function displayProgressBarTemp(currentTemp, tempMin, tempMax) {
  let box = document.getElementById("box");
  let pct = calculatePct(currentTemp, tempMin, tempMax);
  box.style.width = pct + "%";

  if (currentTemp > tempMax) {
    box.classList.add("box-red");
  } else {
    box.classList.remove("box-red");
  }
}

function displayMinMaxTemps(currentTemp, tempMin, tempMax) {
  let dispTempMin = document.getElementById("dispTempMin");
  let dispTempMax = document.getElementById("dispTempMax");
  dispTempMin.innerHTML = tempMin;
  dispTempMax.innerHTML = tempMax;

  if (currentTemp > tempMax) {
    dispTempMax.classList.add("red");
    dispTempMin.classList.remove("blue");
  } else if (currentTemp < tempMin) {
    dispTempMax.classList.remove("red");
    dispTempMin.classList.add("blue");
  } else {
    dispTempMax.classList.remove("red");
    dispTempMin.classList.remove("blue");
  }
}

function displayEngineMessage(motorLastStartTime, motorLastEndTime, motorOn) {
  const engineMessage = document.getElementById("dispEngineMessage");
  if (motorOn) {
    engineMessage.innerHTML = `Motor <span class='green'>ENCENDIDO</span><br>
          <div class='time'>desde las 
          ${motorLastStartTime.slice(0, 2)}:${motorLastStartTime.slice(2)}
          </div>`;
  } else {
    engineMessage.innerHTML = `Motor <span class='gray'>APAGADO</span><br>
          <div class='time'>última vez prendido<br> 
          de ${motorLastStartTime.slice(0, 2)}:${motorLastStartTime.slice(2)} 
          a ${motorLastEndTime.slice(0, 2)}:${motorLastEndTime.slice(2)}
          </div>`;
  }
}

function switchEngineDisplay(motorOn) {
  let engineOn = document.getElementById("engineOn");
  let engineOff = document.getElementById("engineOff");
  if (motorOn) {
    engineOn.classList.remove("hide");
    engineOff.classList.add("hide");
  } else {
    engineOn.classList.add("hide");
    engineOff.classList.remove("hide");
  }
}

function calculatePct(currentTemp, tempMin, tempMax) {
  let pct = ((currentTemp - tempMin) * 100) / (tempMax - tempMin);
  pct = pct < 0 ? 0 : pct;
  pct = pct > 100 ? 100 : pct;
  return pct;
}

function getConfig() {
  result = storage.getItem("backendAddress");
  return result;
}

function configBackendAddress() {
  backendAddress = window.prompt(
    "Escriba la dirección del arduino (ej: http://192.168.50.100)"
  );

  if (backendAddress.slice(-1) === "/") {
    backendAddress = backendAddress.slice(0, -1);
  }
  if (
    backendAddress.slice(0, 7) !== "http://" &&
    backendAddress.slice(0, 8) !== "https://"
  ) {
    backendAddress = "http://" + backendAddress;
  }
  storage.setItem("backendAddress", backendAddress);
  return backendAddress;
}

function parseGet(address) {
  result = address + "/get";

  if (address.slice(-5) == ".json") {
    // hack for develop
    result = address;
  }

  return result;
}

function parseTimeSpans(address) {
  result = address + "/getTimeSpans";

  if (address.slice(-5) == ".json") {
    // hack for develop
    result = address;
  }

  return result;
}

function enableForceEngine(tsForced) {
  const forceDefault = 2;
  const engine = document.getElementById("engine");
  engine.addEventListener("click", () => {
    if (tsForced != forceDefault)
      url = `${backendAddress}/set/force/${forceDefault}`;
    else url = `${backendAddress}/set/resetforce/`;

    fetch(url)
      .then((response) => {
        if (!response.ok) throw new Error();
      })
      .catch((err) => window.alert("Falló al configurar: " + parameter, err));
  });
}

function enableForceButton(tsForced) {
  const param = document.getElementsByClassName("force");
  for (var i = 0; i < param.length; i++) {
    let idTs = param.length - 1 - i;
    let url;
    if (idTs != tsForced) url = `${backendAddress}/set/force/${idTs}`;
    else url = `${backendAddress}/set/resetforce/`;

    param[i].addEventListener("click", () => {
      fetch(url)
        .then((response) => {
          if (!response.ok) throw new Error();
        })
        .catch((err) => window.alert("Falló al configurar: " + parameter, err));
    });
  }
}

function enableModifyParameter(parameterName) {
  const param = document.getElementsByClassName(parameterName);
  for (var i = 0; i < param.length; i++) {
    let idTs = param.length - 1 - i;
    param[i].addEventListener("click", () => setParameter(idTs, parameterName));
  }
}

function setParameter(id, parameter) {
  let input = window.prompt("Escriba un entero para " + id + "/" + parameter);
  if (input) {
    fetch(`${backendAddress}/set/${parameter}/${id}/${input}`)
      .then((response) => {
        if (!response.ok) throw new Error();

        // window.alert(response.text);
      })
      .catch((err) => window.alert("Falló al configurar: " + parameter, err));
  }
}

function setTempMin() {
  let tempMin = window.prompt("Escriba un entero para la temperatura MINIMA");
  fetch(backendAddress + "/set/tempmin/" + tempMin)
    .then((response) => {
      if (!response.ok) throw new Error();

      window.alert(response);
    })
    .catch((err) =>
      window.alert("Falló al configurar la temperatura MINIMA", err)
    );
}
function setTempMax() {
  let tempMax = window.prompt("Escriba un entero para la temperatura MAXIMA");
  fetch(backendAddress + "/set/tempmax/" + tempMax)
    .then((response) => {
      if (!response.ok) throw new Error();

      window.alert(response);
    })
    .catch((err) =>
      window.alert("Falló al configurar la temperatura MAXIMA", err)
    );
}
