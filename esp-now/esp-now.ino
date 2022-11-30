#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Time.h>

const char* ssid = "PruebaAgos";
const char* password = "password";

//MAC Address of receivers
uint8_t broadcastAddress1[] = {0x40,0x91,0x51,0x4D,0xD8,0xDB};
uint8_t broadcastAddress2[] = {0x40,0x91,0x51,0x4D,0xD4,0xE4};
uint8_t broadcastAddress3[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
String latitud;
String longitud;
String latitudDec;
String longitudDec;

bool send = true;
String success;
bool touch = false;
time_t t;
int tiempo;

//Configure to touch
const int ain=A0;
const int LED=4;
int inputVal=0;

float getDistance(){
  int suma = 0;
  for (int i = 0; i < 200;i++){
    Serial.println(WiFi.RSSI());
    int rss = WiFi.RSSI();
    float exponente =  (rss+90)/(-20.0);
    //Serial.println(exponente);
    float distance = pow(10,exponente);    //RSSI(dBm) = -10nlog10(d) + A    PAra d0 el RSSI = -53
    //Serial.println("Distance:\t");
    //Serial.println(distance,1);
    suma = suma + distance;
    //delay(5000); 
  }
  Serial.print("Distance:");  
  Serial.println(suma/200);
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
  Serial.println("Message received: ");
  Serial.println(incomingData[0]);
  digitalWrite(LED, HIGH);
  for(int i=0; i<23; i++){
    Serial.print((char)incomingData[i]);
    } 
   Serial.println();

  latitud = latitud + (char)incomingData[1] + (char)incomingData[2]; 
  for(int i=4; i<11; i++){
    latitudDec = latitudDec + (char)incomingData[i];
    }   
  longitud = longitud + (char)incomingData[12] + (char)incomingData[13];
  for(int i=15; i<23; i++){
    longitudDec = longitudDec + (char)incomingData[i];
    }
  float lat = latitud.toInt() + (latitudDec.toInt()/10000000.0);
  float lon = longitud.toInt() + (longitudDec.toInt()/10000000.0);
  Serial.println(lat,6);
  Serial.println(lon,6);
  lat =0;
  lon =0;

   
   
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
  esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress3, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  //Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Send message via ESP-NOW
   uint8_t STR[] = "349226387579077298";
   
   inputVal=analogRead(ain);
   /*if(inputVal>20)
    {
    Serial.println("TOCAAAAA");*/
    //esp_now_send(0, STR, 10);
    if(send){
    //esp_now_send(broadcastAddress2, STR, 22);
    send = false;
    
    }
    
    
    digitalWrite(LED, HIGH);
    delay(10000);
    digitalWrite(LED, LOW);
    Serial.println("Mac address: "+WiFi.macAddress());
    //for serial monitor
    digitalWrite(LED, LOW);
    delay(15);
  
}
