#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Time.h>
#define PI 3.1415926535897932384626433832795

float getDistance(float,float);

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

bool enviar = true;
String success;
bool touch = false;
time_t t;
int tiempo;

//Configure to touch
const int ain=A0;
const int LED=4;
int inputVal=0;

float getDistance(float lat2, float lon2){
  float d = 0;
  Serial.println("Distaance");
  Serial.println(d);
  float lat1 = 34.922740;
  float lon1 = 57.8075268;
  float φ1 = lat1 * PI/180.0; 
  float φ2 = lat2 * PI/180.0; 
  float Δλ = (lon2-lon1) * PI/180.0;
  int R = 6371000.0;
  d = acos( sin(φ1)*sin(φ2) + cos(φ1)*cos(φ2) * cos(Δλ) ) * R;
  Serial.println("Distaance");
  Serial.println(d);
  }

/*float getDistance(){
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
}*/

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
  float lat2 = latitud.toInt() + (latitudDec.toInt()/10000000.0);
  float lon2 = longitud.toInt() + (longitudDec.toInt()/10000000.0);
  Serial.println(lat2,6);
  Serial.println(lon2,6);
  //float distance = getDistance(lat,lon);
  lat2 = lat2*(PI/180.0);
  lon2 = lon2*(PI/180.0); 
  float d = 0;
  float lat1 = 0;
  float lon1 = 0;
  float φ1 = 0;
  float φ2 = 0;
  float Δφ = 0;
  float Δλ = 0;

  //34.9226441 57.907717"
  lat1 = (34.9226093)*(PI/180.0);
  lon1 = (57.9077889)*(PI/180.0);
  float dLat = lat2 - lat1;
  float dLon = lon2 - lon1;
  float R = 6371000;
  float a = 
    sin(dLat/2) * sin(dLat/2) +
    cos(lat1) * cos(lat2) * 
    sin(dLon/2) * sin(dLon/2)
    ; 
  float c = 2*atan2(sqrt(a),sqrt(1-a)); 
  d = c; // Distance in km
  Serial.println("Distaance");
  Serial.println(d,20);

   lat2 =0;
  lon2 =0;
   
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
  esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress3, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  //Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Send message via ESP-NOW
   uint8_t STR[] = " 34.9226441 57.907717";
   
   inputVal=analogRead(ain);
   
   /*if(inputVal>20)
    {
    Serial.println("TOCAAAAA");*/
    //esp_now_send(0, STR, 10);
    if(enviar){
      Serial.println("Entra");
      //esp_now_send(broadcastAddress2, STR, 22);
      enviar = false;
    
    }
    delay(15);
  
}
