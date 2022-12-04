#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Time.h>

const char* ssid = "PruebaAgos";
const char* password = "password";

//MAC Address of receivers
//uint8_t broadcastAddress1[] = {0x40,0x91,0x51,0x4D,0xD8,0xDB};
uint8_t broadcastAddress3[] = {0x40,0x91,0x51,0x4D,0xD4,0xE4};
uint8_t broadcastAddress1[] = {0x5C,0xCF,0x7F,0x5A,0x4F,0x6C};
String success;
bool touch = false;
time_t t;
int tiempo;
bool inicio = true;
bool ready = false;
float distance = 0;

//Configure to touch
const int ain=A0;
const int LED=4;
int inputVal=0;

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
  Serial.println("Message received: ");
  Serial.println(incomingData[0]);
  digitalWrite(LED, HIGH);

  //On ready message
  if(incomingData == "READY"){
   if((!ready)&&(cantReady==3)){
      ready = true;
    }    
  }
 
  }

void setup() {
  // put your setup code here, to run once:
  //Wifi connection
  Serial.begin(9600);
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
  //esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress3, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  //Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (inicio){
    float suma = 0;
    for(int i=0; i< 200; i++){
      int rss = WiFi.RSSI();
      float exponente =  (rss+42)/(-20.0);
      //Serial.println(exponente);
      distance = pow(10,exponente);    //RSSI(dBm) = -10nlog10(d) + A    PAra d0 el RSSI = -53
      suma += distance;

    }
    distance = suma/200.0;
    Serial.println("Distance:\t");
    Serial.println(distance,3);
    delay(5000);
    inicio = false;
    cantReady++;
    esp_now_send(0, "READY", 10);
  }else{

    if(ready){
      //Send message via ESP-NOW
      uint8_t STR[] = "HELLO";
      /*inputVal=analogRead(ain);
      if(inputVal>20)
      {
      Serial.println("TOCAAAAA");*/
      esp_now_send(0, STR, 10);
      //esp_now_send(broadcastAddress2, STR, 10);
      digitalWrite(LED, HIGH);
      delay(2000);
      digitalWrite(LED, LOW);
      //}
      //Serial.println("Mac address: "+WiFi.macAddress());
      //for serial monitor
      digitalWrite(LED, LOW);
      delay(15);
    }
    
  }

  
  
}
