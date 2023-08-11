const char* PARAM_FILTER = "filter";

void getAll(AsyncWebServerRequest *request)
{
  String message = SerializeMonitoring() ;
  //  Serial.println(message);
  request->send(200, "application/json", message);
}

void getTimeSpans(AsyncWebServerRequest *request)
{
  String message = SerializeTimeSpans() ;
  request->send(200, "application/json", message);
}

void getFiltered(AsyncWebServerRequest *request)
{
  String message = "Get filtered by " + request->getParam(PARAM_FILTER)->value();
  Serial.println(message);
  request->send(200, "text/plain", message);
}

void getById(AsyncWebServerRequest *request)
{
  int id = GetIdFromURL(request, "/item/");

  String message = String("Get by Id ") + id;
  Serial.println(message);
  request->send(200, "text/plain", message);
}

void getRequest(AsyncWebServerRequest *request) {
  if (request->hasParam(PARAM_FILTER)) {
    getFiltered(request);
  }
  else if (request->url().indexOf("/item/") != -1)
  {
    getById(request);
  }
  else {
    getAll(request);
  }
}

void setStartTime(AsyncWebServerRequest *request) {
  String temp = GetStringFromURL(request, "/set/starttime/");
  char rq[temp.length() + 1];
  temp.toCharArray(rq, temp.length() + 1);

  char *token1;
  char *token2;
  char *delimiter = "/";

  token1 = strtok(rq, delimiter);
  token2 = strtok(NULL, delimiter);

  tsEngine.setStartTime(atoi(token1), String(token2));
  String message = "Start time " + String(token1) + " set to " + String(token2);
  Serial.println(message);
  request->send(200, "text/plain", message);
}

void setEndTime(AsyncWebServerRequest *request) {
  String temp = GetStringFromURL(request, "/set/endtime/");
  char rq[temp.length() + 1];
  temp.toCharArray(rq, temp.length() + 1);

  char *token1;
  char *token2;
  char *delimiter = "/";

  token1 = strtok(rq, delimiter);
  token2 = strtok(NULL, delimiter);

  tsEngine.setEndTime(atoi(token1), String(token2));
  String message = "End time " + String(token1) + " set to " + String(token2);
  Serial.println(message);
  request->send(200, "text/plain", message);
}

void setTempMin(AsyncWebServerRequest *request) {
  String temp = GetStringFromURL(request, "/set/tempmin/");
  char rq[temp.length() + 1];
  temp.toCharArray(rq, temp.length() + 1);

  char *token1;
  char *token2;
  char *delimiter = "/";

  token1 = strtok(rq, delimiter);
  token2 = strtok(NULL, delimiter);

  tsEngine.setTempMin(atoi(token1), atoi(token2));
  String message = "Min temperature " + String(token1) + " set to " + String(token2);
  Serial.println(message);
  request->send(200, "text/plain", message);
}

void setTempMax(AsyncWebServerRequest *request) {
  String temp = GetStringFromURL(request, "/set/tempmax/");
  char rq[temp.length() + 1];
  temp.toCharArray(rq, temp.length() + 1);

  char *token1;
  char *token2;
  char *delimiter = "/";

  token1 = strtok(rq, delimiter);
  token2 = strtok(NULL, delimiter);

  tsEngine.setTempMax(atoi(token1), atoi(token2));
  String message = "Max temperature " + String(token1) + " set to " + String(token2);
  Serial.println(message);
  request->send(200, "text/plain", message);
}

void setTsName(AsyncWebServerRequest *request) {
  String temp = GetStringFromURL(request, "/set/name/");
  char rq[temp.length() + 1];
  temp.toCharArray(rq, temp.length() + 1);

  char *token1;
  char *token2;
  char *delimiter = "/";

  token1 = strtok(rq, delimiter);
  token2 = strtok(NULL, delimiter);

  tsEngine.setTimeSpanName(atoi(token1), String(token2));
  String message = "New name " + String(token1) + " set to " + String(token2);
  Serial.println(message);
  request->send(200, "text/plain", message);
}

void setEnabled(AsyncWebServerRequest *request) {
  String temp = GetStringFromURL(request, "/set/enabled/");
  char rq[temp.length() + 1];
  temp.toCharArray(rq, temp.length() + 1);

  char *token1;
  char *token2;
  char *delimiter = "/";

  token1 = strtok(rq, delimiter);
  token2 = strtok(NULL, delimiter);

  bool result = String(token2) != "false";

  tsEngine.setEnabled(atoi(token1), result);
  String message = "Timespan enabled " + String(token1) + " set to " + String(token2);
  Serial.println(message);
  Serial.println(result);
  request->send(200, "text/plain", message);
}

void setForce(AsyncWebServerRequest *request) {
  int id = GetIdFromURL(request, "/set/force/");

  tsEngine.forceTimeSpan(id);
  String message = "Timespan " + String(id) + " forced to run up to tempMin / tempMax reached";
  Serial.println(message);
  request->send(200, "text/plain", message);
}

void resetForce(AsyncWebServerRequest *request) {
  int id = GetIdFromURL(request, "/set/resetforce/");

  tsEngine.resetForcedTimeSpan();
  String message = "Timespan reset to the timespan and priority operation";
  Serial.println(message);
  request->send(200, "text/plain", message);
}

//void freeRam(AsyncWebServerRequest *request) {
//  String message = String(ESP.getFreeHeap());
//  Serial.println("Free memory: " + message);
//  request->send(200, "text/plain", message);
//}
//void setTempMax(AsyncWebServerRequest *request) {
//  float temp = float(GetIdFromURL(request, "/set/tempmax/"));
//
//  if (temp != -1)
//  {
////    brain.setTempMax(temp);
//    String message = "Max temperature set to " + String(temp);
//    Serial.println(message);
//    request->send(200, "text/plain", message);
//  } else {
//    getAll(request);
//  }
//}
//
//void setTempMin(AsyncWebServerRequest *request) {
//  float temp = float(GetIdFromURL(request, "/set/tempmin/"));
//
//  if (temp != -1)
//  {
////    brain.setTempMin(temp);
//    String message = "Min temperature set to " + String(temp);
//    Serial.println(message);
//    request->send(200, "text/plain", message);
//  } else {
//    getAll(request);
//  }
//}

// -----

String SerializeMonitoring()
{
  String json;
  StaticJsonDocument<300> doc;
  doc["currentTemp"] = sensors.getTempCByIndex(0);
  doc["motorOn"] = brain.isMotorOn();
  doc["backendVersion"] = brain.getBackendVersion();
  doc["monitoringMode"] = brain.getmonitoringMode();
  doc["motorStartTime"] = brain.getMotorLastStartTime();
  doc["motorEndTime"] = brain.getMotorLastEndTime();
  doc["timeSpanid"] = tsEngine.getCurrentTimeSpanId();
  doc["timeSpanName"] = tsEngine.getTimeSpanName();
  doc["tempMax"] = tsEngine.getTempMax();
  doc["tempMin"] = tsEngine.getTempMin();
  doc["tsForced"] = tsEngine.forcedTs();
  
  serializeJson(doc, json);
  return json;
}

String SerializeTimeSpans()
{
  String json;
  StaticJsonDocument<650> doc;
  doc["time"] = String(timeClient.getHours()*100+timeClient.getMinutes());
  JsonArray timeSpans = doc.createNestedArray("timeSpans");
  TimeSpanData *tsData = tsEngine.periods;
  for (int i = 0; i < numPeriods; i++)
  {
    JsonObject doc_int;
    doc_int = timeSpans.createNestedObject();
    doc_int["id"] = tsData[i].id;
    doc_int["tsName"] = tsData[i].tsName;
    doc_int["startTime"] = tsData[i].startTime;
    doc_int["endTime"] = tsData[i].endTime;
    doc_int["tempMin"] = tsData[i].tempMin;
    doc_int["tempMax"] = tsData[i].tempMax;
    doc_int["enabled"] = tsData[i].enabled;
  }
  serializeJson(doc, json);
  return json;
}
// ----
