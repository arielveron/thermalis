#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>


// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D6
#define RELE D1


OneWire oneWire(ONE_WIRE_BUS);


DallasTemperature sensors(&oneWire);

// Red WiFi Configuración

const char* ssid = "KameHouse";
const char* password = "2307wifi.";

IPAddress ip (192, 168, 50, 100);
IPAddress gateway (192, 168, 50, 1);
IPAddress subnet (255, 255, 255, 0);

class NASABrain {
  private:
    float tempMin;
    float tempMax;
    bool motorOn;
    // actuate = false for helaNasa
    // actuate = true for termoNasa
    bool actuate = true;
    
  public:
    NASABrain(float tMin, float tMax){
      tempMin = tMin;
      tempMax = tMax;
      motorOn = false;
    }

    bool isMotorOn(){
      return motorOn;
    }

    bool update(float currentTemp){
      if (currentTemp < tempMin) {
        motorOn = actuate;
      }
      if (currentTemp > tempMax) {
        motorOn = !actuate; 
      }
      return motorOn;
    }

    void setTempMax(float tMax) {
      tempMax = tMax;
    }

    void setTempMin(float tMin) {
      tempMin = tMin;
    }

    float getTempMax() {
      return tempMax;
    }

    float getTempMin() {
      return tempMin;
    }

    void getTemps(){
      Serial.print("Min: ");
      Serial.println(tempMin);
      Serial.print("Max:");
      Serial.println(tempMax);
    }
};

NASABrain brain(24.0, 25.0);

// --- Server.hpp ----
// -------------------

AsyncWebServer server(80);

void homeRequest(AsyncWebServerRequest *request) {
  request->send(200, "text/plain", "Hello, world");
}

void notFound(AsyncWebServerRequest *request) {
   request->send(404, "text/plain", "Not found");
}

void InitServer()
{
   server.on("/", HTTP_GET, homeRequest);
   server.on("/get", HTTP_GET, getRequest);
   server.on("/set/tempmax", HTTP_GET, setTempMax);
   server.on("/set/tempmin", HTTP_GET, setTempMin);
   
   server.onNotFound(notFound);

   DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

   server.begin();
    Serial.println("HTTP server started");
}

// -------

// ----- API.hpp ---
// -----------------
// #include "ESP8266_Utils_APIREST.hpp"

const char* PARAM_FILTER = "filter";

void getAll(AsyncWebServerRequest *request)
{
  String message = SerializeObject() ;
  Serial.println(message);
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
  else if(request->url().indexOf("/item/") != -1)
  {
    getById(request);
  }
  else {
    getAll(request);
  }
}

void setTempMax(AsyncWebServerRequest *request) {
  
  float temp = float(GetIdFromURL(request, "/set/tempmax/"));
  
  if(temp != -1)
  {
    brain.setTempMax(temp);
    String message = "Max temperature set to " + String(temp);
    Serial.println(message);
    request->send(200, "text/plain", message);
  } else {
    getAll(request);
  }
}

void setTempMin(AsyncWebServerRequest *request) {


  float temp = float(GetIdFromURL(request, "/set/tempmin/"));
  
  if(temp != -1)
  {
    brain.setTempMin(temp);
    String message = "Min temperature set to " + String(temp);
    Serial.println(message);
    request->send(200, "text/plain", message);
  } else {
    getAll(request);
  }
}

// -----

// ---- ESP8266_Utils_APIREST.hpp
// -----------

void postRequest(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total)
{ 
  String bodyContent = GetBodyContent(data, len);
  
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, bodyContent);
  if (error) { request->send(400); return;}

  String string_data = doc["data"];
  String message = "Create " + string_data;
  Serial.println(message);
  request->send(200, "text/plain", message);
}

void patchRequest(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total)
{
  int id = GetIdFromURL(request, "/item/");
  String bodyContent = GetBodyContent(data, len);
  
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, bodyContent);
  if (error) { request->send(400); return;}

  String string_data = doc["data"];
  String message = String("Update ") + id + " with " + string_data;
  Serial.println(message);
  request->send(200, "text/plain", message);
}

void putRequest(AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total)
{
  int id = GetIdFromURL(request, "/item/");
  String bodyContent = GetBodyContent(data, len);
   
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, bodyContent);
  if (error) { request->send(400); return;}

  String string_data = doc["data"];
  String message = String("Replace ") + id + " with " + string_data;
  Serial.println(message);
  request->send(200, "text/plain", message);
}

void deleteRequest(AsyncWebServerRequest *request) {
  int id = GetIdFromURL(request, "/item/");

  String message = String("Delete ") + id;
  Serial.println(message);
  request->send(200, "text/plain", message);
}

int GetIdFromURL(AsyncWebServerRequest *request, String root)
{
  String string_id = request->url();
  string_id.replace(root, "");
  int id = string_id.toInt();
  return id;
}

String GetBodyContent(uint8_t *data, size_t len)
{
  String content = "";
  for (size_t i = 0; i < len; i++) {
    content .concat((char)data[i]);
  }
  return content;
}

// -----

String SerializeObject()
{
    String json;
    StaticJsonDocument<300> doc;
    doc["currentTemp"] = sensors.getTempCByIndex(0);
    doc["motorOn"] = brain.isMotorOn();
    doc["tempMax"] = brain.getTempMax();
    doc["tempMin"] = brain.getTempMin();
    serializeJson(doc, json);
    Serial.println(json);
    return json;
}
// ----


/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{
  // WIFI ---
  server.begin();

  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  Serial.println("");
  Serial.println("Red WiFi conectada");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // ---
  
  // start serial port
  Serial.begin(115200);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();

  pinMode(RELE, OUTPUT);
  
  // ConnectWiFi_STA();
  InitServer();

}

/*
 * Main function, get and show the temperature
 */
void loop(void)
{ 
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.print("Temperature for the device 1 (index 0) is: ");
  float temp = sensors.getTempCByIndex(0);
  Serial.println(temp);

  /// ----
  if (brain.isMotorOn()) {
    digitalWrite(RELE, HIGH);  
  } else {
    digitalWrite(RELE, LOW);  
  }

  // ----
  brain.getTemps();
  Serial.println(brain.isMotorOn());
  Serial.println(brain.update(temp));
  Serial.println(brain.isMotorOn());
}
