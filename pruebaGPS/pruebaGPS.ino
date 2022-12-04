#include <ESP8266HTTPClient.h>

#include <ArduinoJson.h>

#include "ESP8266WiFi.h"

char ssid[] = "Telecentro-0151";         // your network SSID name

char pass[] = "2051306BUNKER";          // your network password


//Credentials for Google GeoLocation API...

const char* Host = "www.googleapis.com";

String thisPage = "/geolocation/v1/geolocate?key=";

String key = "AIzaSyAUY9b0xkqVJCDkY7wHILLnGzwmvCaK3O4";

int status = WL_IDLE_STATUS;

String jsonString = "{\n";

double latitude    = 0.0;

double longitude   = 0.0;

double accuracy    = 0.0;

int more_text = 1;    // set to 1 for more debug output


void setup()   {

  Serial.begin(9600);

  Serial.println("Start");


 // Set WiFi to station mode and disconnect from an AP if it was previously connected

  WiFi.mode(WIFI_STA);

  WiFi.disconnect();

  delay(100);

  Serial.println("Setup done");

  

// We start by connecting to a WiFi network

  Serial.print("Connecting to ");

  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");

  }

  Serial.println(".");


}


void loop() {

  getData();

}


void getData(){
  char bssid[6];

  DynamicJsonBuffer jsonBuffer;

  Serial.println("scan start");

  

// WiFi.scanNetworks will return the number of networks found

  int n = WiFi.scanNetworks();


// now build the jsonString...

  jsonString = "{\n";

  jsonString += "\"homeMobileCountryCode\": 722,\n"; // this is a real UK MCC

  jsonString += "\"homeMobileNetworkCode\": 7,\n";  // and a real UK MNC

  jsonString += "\"radioType\": \"gsm\",\n";         // for gsm

  jsonString += "\"carrier\": \"Vodafone\",\n";      // associated with Vodafone

  jsonString += "\"wifiAccessPoints\": [\n";

  for (int j = 0; j < n; ++j)

  {

    jsonString += "{\n";

    jsonString += "\"macAddress\" : \"";

    jsonString += (WiFi.BSSIDstr(j));

    jsonString += "\",\n";

    jsonString += "\"signalStrength\": ";

    jsonString += WiFi.RSSI(j);

    jsonString += "\n";

    if (j < n - 1)

    {

      jsonString += "},\n";

    }

    else

    {

      jsonString += "}\n";

    }

  }

  jsonString += ("]\n");

  jsonString += ("}\n");

  //-------------------------------------------------------------------- Serial.println("");

  //Connect to the client and make the api call



  WiFiClientSecure client;

    client.setInsecure();

  Serial.print("Requesting URL: ");

  Serial.println("https://" + (String)Host + thisPage + key);

  Serial.println(" ");

  if (client.connect(Host, 443)) {

    Serial.println("Connected");

    client.println("POST " + thisPage + key + " HTTP/1.1");

    client.println("Host: " + (String)Host);

    client.println("Connection: close");

    client.println("Content-Type: application/json");

    client.println("User-Agent: Arduino/1.0");

    client.print("Content-Length: ");

    client.println(jsonString.length());

    client.println();

    client.print(jsonString);

    delay(500);

  }

  //Read and parse all the lines of the reply from server

  while (client.available()) {

    String line = client.readStringUntil('\r');

    if (more_text) {

      Serial.print(line);

    }

    JsonObject& root = jsonBuffer.parseObject(line);

    if (root.success()) {

      latitude    = root["location"]["lat"];

      longitude   = root["location"]["lng"];

      accuracy   = root["accuracy"];

    }

  }

  Serial.println("closing connection");

  Serial.println();

  client.stop();

  Serial.print("Latitude = ");

  Serial.println(latitude, 6);

  Serial.print("Longitude = ");

  Serial.println(longitude, 6);

  Serial.print("Accuracy = ");

  Serial.println(accuracy);
  }
