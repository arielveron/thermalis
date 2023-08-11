#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESPAsyncWebServer.h>

#include "config.h"

//Selecciona el pin al que se conecta el sensor de temperatura
const int oneWireBus = 4;


// Initial setup
OneWire oneWire(oneWireBus);
DallasTemperature sensors (&oneWire);
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
     
  WiFi.mode(WIFI_STA);
  //  WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(ssid, password);

  sensors.begin();
  
//  server.begin();
  InitServer();

//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }

//  Serial.println("");
//  Serial.println("Red WiFi conectada");
//  Serial.println("Dirección IP: ");
//  Serial.println(WiFi.localIP());
}

void loop() {
  //Leer la temperatura
  Serial.print("Mandando comandos a los sensores ");
  sensors.requestTemperatures();
  //Lectura en grados celsius
  float temperatureC = sensors.getTempCByIndex(0);
  //Escribir los datos en el monitor de serie
  Serial.print("Temperatura sensor : ");
  Serial.print(temperatureC);
  Serial.println("°C");
// Lectura de la temperatura cada 1 segundos
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
  delay(1000);

}
