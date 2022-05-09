#include <Arduino.h>
#include "WiFi.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"

const char* apSSID = "ESP-Epaper";
const char* apPassword = "123456789";

AsyncWebServer server(80);
StaticJsonDocument<1024> wifiNetworks;

String readFile(String filename){
  File f = SPIFFS.open(filename);
  if(!f){
    Serial.print("Failed to open file: ");
    Serial.println(filename);
    return "";
  }
  String buffer;
  while(f.available()){
    buffer+=static_cast<char>(f.read());
  }
  f.close();
  return buffer;
}

void setup() {
  Serial.begin(9600);

  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
  }

  String buffer = readFile("/WiFi_Credentials.json");
  DeserializationError error = deserializeJson(wifiNetworks, buffer);
  if(error){
    Serial.println("Deserializing WiFi_Credentials.json failed");
    Serial.println(error.f_str());
    return;
  }

  JsonArray arr = wifiNetworks.as<JsonArray>();
  for(JsonVariant v : arr){
    Serial.println(v["SSID"].as<const char*>());
  }

  WiFi.softAP(apSSID, apPassword);

  server.on("/html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/setup.html", "text/html");
  });

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}