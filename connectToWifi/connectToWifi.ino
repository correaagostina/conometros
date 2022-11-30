#include <ESP8266WiFi.h>

// Sustituir con datos de vuestra red
const char *ssid_AP = "ESP8266_AP1";
const char *password_AP = "12345678";
const char *ssid_STA = "mi_ssid_local";
const char *password_STA = "????????";

void setup()
{
  Serial.begin(9600);
  delay(10);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid_AP, password_AP);
  WiFi.begin(ssid_STA, password_STA);
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}  

  // Esperar a que nos conectemos
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(200);
   Serial.print('.');
  }
  // Mostrar mensaje de exito y dirección IP asignada
  Serial.println();
  Serial.print("Conectado a:\t");
  Serial.println(WiFi.SSID()); 
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  
}

void loop() 
{
  Serial.println(WiFi.RSSI());
  int rss = WiFi.RSSI();
  float exponente =  (rss+70)/(-20.0);
  Serial.println(exponente);
  float distance = pow(10,exponente);    //RSSI(dBm) = -10nlog10(d) + A    PAra d0 el RSSI = -53
  Serial.println("Distance:\t");
  Serial.println(distance,1);
  delay(5000);
}
