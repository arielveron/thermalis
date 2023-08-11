#include <ESP8266WiFi.h>
//#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "config.h"

// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D6
#define RELE D1
#define FORCE_BUTTON D5

#define numPeriods 3
#define defaultForcedTimespan 2

// enabled physical force button
const String backendVersion = "2.2.6";

#include "structs.h"
#include "timeSpanEngine.h"
#include "nasaBrain.h"

float oldTemp = -127.0;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -10802, 60000);

// ------
// Set Initial Parameters for this specific NASA implementation
// ------

TimeSpanData initialData = (TimeSpanData) {
  .tsName = "Default",
  .startTime = "0000",
  .endTime = "0000", 
  .tempMin = 28, 
  .tempMax = 35, 
  .enabled = true
};

TimeSpanEngine tsEngine(initialData);
//(TimeSpanData) {"Noche", "1845","1848", 20, 24, true}
TimeSpanData night = (TimeSpanData) {
  .tsName = "Noche",
  .startTime = "2300",
  .endTime = "1100", 
  .tempMin = 10, 
  .tempMax = 11, 
  .enabled = true
};
TimeSpanData rush = (TimeSpanData) {
  .tsName = "Rush",
  .startTime = "1500",
  .endTime = "2300", 
  .tempMin = 38, 
  .tempMax = 41, 
  .enabled = true
};

NASABrain brain(&tsEngine, TERMONASA);
AsyncWebServer server(80);

// ------

void setup(void)
{
  // WIFI ---
  server.begin();

  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(ssid, password);

  Serial.println("");
  Serial.println("Red WiFi conectada");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // start serial port
   Serial.begin(115200);

  // Start up the library
  sensors.begin();
  pinMode(RELE, OUTPUT);

  // ConnectWiFi_STA();
  InitServer();
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Sync clock
  timeClient.begin();

  tsEngine.setTimeSpan(1, night);
  tsEngine.setTimeSpan(2, rush);

  pinMode(FORCE_BUTTON, INPUT);
}

/*
   Main function, get and show the temperature
*/
void loop(void)
{
  sensors.requestTemperatures(); // Send the command to get temperatures
  timeClient.update();

  float temp = sensors.getTempCByIndex(0);
  float deltaTemp = oldTemp - temp;
  // print debub info
  Serial.println(temp);
  Serial.println(deltaTemp);
  Serial.println(timeClient.getFormattedTime());
  Serial.println(WiFi.localIP());

  if (digitalRead(FORCE_BUTTON) == HIGH) {
    tsEngine.forceTimeSpan(defaultForcedTimespan);
  } 

  /// ----
  if (brain.isMotorOn()) {
    digitalWrite(RELE, HIGH);
  } else {
    digitalWrite(RELE, LOW);
  }

  // ----
  //    brain.getTemps();
  tsEngine.update(timeClient.getHours(), timeClient.getMinutes());
  brain.update(temp);
  oldTemp = temp;
}
