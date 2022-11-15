#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Time.h>

//MAC Address of receivers
uint8_t broadcastAddress1[] = {0x40,0x91,0x51,0x4D,0xD8,0xDB};
uint8_t broadcastAddress2[] = {0x40,0x91,0x51,0x4D,0x6C,0xE9};
String success;
bool touch = false;
time_t t;
int tiempo;

//Configure to touch
const int ain=A0;
const int LED=2;
int inputVal=0;

float getDistance(){
  Serial.println(WiFi.RSSI());
  int rss = WiFi.RSSI();
  float exponente =  (rss+90)/(-20.0);
  Serial.println(exponente);
  float distance = pow(10,exponente);    //RSSI(dBm) = -10nlog10(d) + A    PAra d0 el RSSI = -53
  Serial.println("Distance:\t");
  Serial.println(distance,1);
  delay(5000);
  }

//Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus){
  Serial.printf("Last packet send status: ");
  if(sendStatus == 0){
    Serial.println("Delivery success");
    }
    else{
      Serial.println("Delivery fail");
      }
  }

//Callback when data is received
void OnDataRecv(uint8_t* mac, uint8_t *incomingData, uint8_t len){
  if(touch){
    //tiempo = t - now();
    touch = false;
    //Serial.println("TIEMPO TRANSCURRIDO: ", tiempo);
  }else{
    //t = now();
    }
  
  Serial.println("Message received: ");
  Serial.println(incomingData[0]);
  //received = true;
//  esp_now_send(0, STR, 10);
  }

void setup() {
  // put your setup code here, to run once:
  //Wifi connection
  Serial.begin(9600);
  Serial.print("Llega");
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  Serial.println("Mac address: "+WiFi.macAddress());
  /*float distance = */getDistance();
  WiFi.disconnect();

  //Setting pins to touch
  pinMode(LED,OUTPUT);
  Serial.begin(9600);
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
  esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  //Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Send message via ESP-NOW
   uint8_t STR[] = "HELLO";
   inputVal=analogRead(ain);
   if(inputVal>20)
    {
    Serial.println("TOCAAAAA");
    esp_now_send(0, STR, 10);
    //esp_now_send(broadcastAddress2, STR, 10);
    delay(1000);
    }
    Serial.println("Mac address: "+WiFi.macAddress());
    //for serial monitor
    delay(15);
  
}
