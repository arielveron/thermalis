void homeRequest(AsyncWebServerRequest *request) {
  float temp = sensors.getTempCByIndex(0);
  request->send(200, "text/plain", String(temp));
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void InitServer()
{
  server.on("/", HTTP_GET, homeRequest);
  server.on("/get", HTTP_GET, getRequest);
//  server.on("/getTimeSpans", HTTP_GET, getTimeSpans);
//  server.on("/set/starttime", HTTP_GET, setStartTime);
//  server.on("/set/endtime", HTTP_GET, setEndTime);
//  server.on("/set/tempmax", HTTP_GET, setTempMax);
//  server.on("/set/tempmin", HTTP_GET, setTempMin);
//  server.on("/set/name", HTTP_GET, setTsName);
//  server.on("/set/enabled", HTTP_GET, setEnabled);
//  server.on("/set/force", HTTP_GET, setForce);
//  server.on("/set/resetforce", HTTP_GET, resetForce);
//  server.on("/get/freeRam", HTTP_GET, freeRam);

  server.onNotFound(notFound);

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

  server.begin();
  Serial.println("HTTP server started");
}
