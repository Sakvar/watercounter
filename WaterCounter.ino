/*
 *  TODO: choose password depending on WiFi SSID
 *  Prefered SSIDs and passwords are hardcoded (to be changed)
 */
#include "ESP8266WiFi.h"
#include "ESP8266mDNS.h"

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(2, OUTPUT);
  digitalWrite(2, 0);pinMode(2, OUTPUT);
  digitalWrite(2, 0);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");

    int bestRSSI = -200;
    int bestI;
    
    for (int i = 0; i < n; ++i)
    { 
      if ((WiFi.SSID(i) == "!Hotspot")|(WiFi.SSID(i) == "mgts335")){
        if (WiFi.RSSI(i) > bestRSSI) {
          bestRSSI = WiFi.RSSI(i);
          bestI = i;
        }
      // Print SSID and RSSI for each network found
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
        delay(10);
      }
    }
    Serial.println("");
    Serial.print("The best signal is ");
    Serial.print(WiFi.SSID(bestI));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(bestI));
    Serial.print(")");
    Serial.println((WiFi.encryptionType(bestI) == ENC_TYPE_NONE)?" ":"*");

    WiFi.hostname("espnode");
    
    WiFiConnect(WiFi.SSID(bestI), "fuckingpassword"); //mgts335, 19052015
    //WiFiConnect("mgts335", "19052015"); //mgts335, 19052015
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);

  server.begin();
  Serial.println("Server started");
  
  Serial.println("Setup done");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  int val;
  if (req.indexOf("/gpio/0") != -1)
    val = 0;
  else if (req.indexOf("/gpio/1") != -1)
    val = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(2, val);
  
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now ";
  s += (val)?"high":"low";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

void WiFiConnect(String ssid, String password){
  WiFi.mode(WIFI_STA);
  //Serial.println(ssidstring);
  //Serial.println(passwordstring);
  WiFi.begin(ssid.c_str(), password.c_str());
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

