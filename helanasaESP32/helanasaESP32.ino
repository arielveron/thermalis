#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#define numPeriods 3
#define defaultForcedTimespan 2
const String backendVersion = "2.0.4";
//Selecciona el pin al que se conecta el sensor de temperatura
const int oneWireBus = 4;
const int RELE = 18;

// Initial setup
OneWire oneWire(oneWireBus);
DallasTemperature sensors (&oneWire);
AsyncWebServer server(80);

#include "config.h"
#include "structs.h"
#include "timeSpanEngine.h"
#include "nasaBrain.h"

TimeSpanData initialData = (TimeSpanData) {
  .tsName = "Default",
  .startTime = "0000",
  .endTime = "0000",
  .tempMin = 4,
  .tempMax = 7,
  .enabled = true
};

TimeSpanEngine tsEngine(initialData);
NASABrain brain(&tsEngine, HELANASA);

#include "serialize.h"
SerializeAPI serialize(&sensors, &tsEngine, &brain);

#include "api.h"


void setup() {
  Serial.begin(115200);
     
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(ssid, password);

  pinMode(RELE, OUTPUT);

  sensors.begin();
  
//  server.begin();
  InitServer();

while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
}

  Serial.println("");
  Serial.println("Red WiFi conectada");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //Leer la temperatura
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);

  // tsEngine.update(timeClient.getHours(), timeClient.getMinutes());
  brain.update(temperatureC);

  // ----
  if (brain.isMotorOn()) {
    digitalWrite(RELE, HIGH);
  } else {
    digitalWrite(RELE, LOW);
  }

  // ----  
  //Escribir los datos en el monitor de serie
  Serial.print("Temperatura sensor : ");
  Serial.print(temperatureC);
  Serial.println("°C");
  
  Serial.print("Motor status: ");
  Serial.println(brain.isMotorOn());
  
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
  
  delay(1000);
  
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
    }
  }
