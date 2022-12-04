
//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266WIFI_LIB
#include <ESP8266WiFi.h>

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "Agos"
#define REMOTEXY_WIFI_PASSWORD "password"
#define REMOTEXY_SERVER_PORT 6377

// Set your Static IP address
IPAddress local_IP(192, 168, 4, 2);
// Set your Gateway IP address
IPAddress gateway(192, 168, 4, 1);

IPAddress subnet(255, 255, 255, 0);


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 27 bytes
  { 255,1,0,0,0,20,0,16,8,1,10,48,24,40,15,15,4,36,31,79,
  78,0,31,79,70,70,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t pushSwitch_1; // =1 if state is ON, else =0 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////


void setup() 
{
  RemoteXY_Init (); 
  Serial.begin(9600);
  WiFi.softAP(REMOTEXY_WIFI_SSID, REMOTEXY_WIFI_PASSWORD);
  WiFi.config(local_IP, gateway, subnet);
 
  // TODO you setup code
  
}

void loop() 
{ 
  Serial.println(WiFi.localIP());
  RemoteXY_Handler ();
  if(RemoteXY.pushSwitch_1==1){
    Serial.println("ESTA ENCENDIDO");
    
  }else{
    Serial.println("ESTA APAGADO");
  }
  delay(1000);  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay() 


}
