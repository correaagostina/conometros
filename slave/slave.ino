#include<ESP8266WiFi.h>
#include<espnow.h>

#define MY_NAME   "SLAVE_NODE"

struct __attribute__((packed)) dataPacket {
  int sensor1;
  int sensor2;
  float sensor3;
};

void dataReceived(uint8_t *senderMac, uint8_t *data, uint8_t dataLength) {
  char macStr[18];
  dataPacket packet;  

  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", senderMac[0], senderMac[1], senderMac[2], senderMac[3], senderMac[4], senderMac[5]);

  Serial.println();
  Serial.print("Received data from: ");
  Serial.println(macStr);
  
  memcpy(&packet, data, sizeof(packet));
  
  Serial.print("sensor1: ");
  Serial.println(packet.sensor1);
  Serial.print("sensor2: ");
  Serial.println(packet.sensor2);
  Serial.print("sensor3: ");
  Serial.println(packet.sensor3);
}
 
void setup() {
    
  Serial.begin(115200);     // initialize serial port

  Serial.println();
  Serial.println();
  Serial.println();
  Serial.print("Initializing...");
  Serial.println(MY_NAME);
  Serial.print("My MAC address is: ");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();        // we do not want to connect to a WiFi network

  if(esp_now_init() != 0) {
    Serial.println("ESP-NOW initialization failed");
    return;
  }

  esp_now_register_recv_cb(dataReceived);   // this function will get called once all data is sent

  Serial.println("Initialized.");
}

void loop() {

}
