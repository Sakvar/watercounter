/*
 *  
 */
#include "ESP8266WiFi.h"

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.println("Setup done");
}

void loop() {
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
    
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
}


