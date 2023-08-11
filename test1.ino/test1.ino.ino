/*
 * ESP32 LED Blink Example
 * Board ESP23 DEVKIT V1
 * 
 * ON Board LED GPIO 2
 */
#define LED 4
void setup() {
  // Set pin mode
  pinMode(LED,OUTPUT);
}
void loop() {
//  digitalWrite(LED,HIGH);
//  delay(2000);
  digitalWrite(LED,LOW);
//  delay(500);
}
