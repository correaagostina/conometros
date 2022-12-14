#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Time.h>
#include <iostream>
#include <fstream>
using namespace std;

float getDistance(float,float);

const char* ssid = "PruebaAgos";
const char* password = "password";

int miMac = 108; 
//MAC Address of receivers
uint8_t broadcastAddress3[] = {0x5C,0xCF,0x7F,0x5A,0x4F,0x6C};
uint8_t broadcastAddress1[] = {0x84,0xf3,0xeb,0xb1,0xc4,0x6e};
//uint8_t broadcastAddress3[] = {0x40,0x91,0x51,0x4D,0xD4,0xE4};

String success;
bool touch = false;
time_t t;
float tiempo;
bool inicio = true;
bool ready = false;
float distance = 0;
bool received = false;
float lastMillis=0;

//Configure to touch
const int ain=A0;
const int LED=4;
int inputVal=0;
bool entra = true;


//Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus){
  Serial.printf("Last packet send status: ");
  if(sendStatus == 0){
    Serial.println("Delivery success");
    lastMillis = millis();
    }
    else{
      Serial.println("Delivery fail");
      }
  }

//Callback when data is received
void OnDataRecv(uint8_t* mac, uint8_t *incomingData, uint8_t len){
  Serial.println("Message received: ");
  //Serial.println(mac[12] + mac[13]);
  digitalWrite(LED, HIGH);

  //if(miMac!=mac[5]){
    if(touch){
      tiempo = millis() - lastMillis;
      Serial.print("Tiempo desde ESP : ");  
      for(int i=0; i<6;i++){
        Serial.print(mac[i],HEX); 
        Serial.print(" :");   
      }
      Serial.print(" hasta ESP : "); 
      Serial.print(WiFi.macAddress());
      Serial.println(); 
      Serial.println(tiempo/1000,3);    
      touch = false; 
      tiempo = 0;
      lastMillis = 0; 
    }
      
  
   
  //}

   
}
void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  //Wifi connection
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Conectando a:\t");
  Serial.println(ssid); 
  Serial.println("Mac address: "+WiFi.macAddress());

  Serial.print("Conectado a:\t");
  Serial.println(WiFi.SSID()); 
  WiFi.disconnect();

  //Setting pins to touch
  pinMode(LED,OUTPUT);
  
  //Init ESP-NOW
  if (esp_now_init() != 0){
    Serial.println("Error initializing ESP-NOW");
    return;
    }

  //Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  //Once Esp-now is succcessfully init, we will register for send CB to
  //get the satus of trasnmittd packet
  esp_now_register_send_cb(OnDataSent);

  //Register peer
  //esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress3, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  //Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:

  
  //Send message via ESP-NOW
   uint8_t STR[] = "MENSAJE";
   digitalWrite(LED, LOW);
   inputVal=analogRead(ain);
   
   if(inputVal>15)
    {
          digitalWrite(LED, HIGH);
          Serial.println("Detecta toque.");
          touch = true;
          esp_now_send(0, STR, 10);
          entra = false;        

    
    }
    delay(50);
  
}
