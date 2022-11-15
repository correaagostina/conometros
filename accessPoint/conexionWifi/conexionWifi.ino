#include <ESP8266WiFi.h>

const char* ssid = "PruebaAgos";
const char* password = "password";

void setup() {
  Serial.begin(115200);
  Serial.println("\nSetting AP");
  WiFi.softAP(ssid, password);
  Serial.println("WiFi is ready");

}

void loop() {
  int device = WiFi.softAPgetStationNum();
  Serial.printf("Devices connected = %d\n", device);
  delay(5000);

}
