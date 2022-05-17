#include <Arduino.h>
#include "WiFi.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"
#include "networks.hpp"

const char* apSSID = "ESP-Epaper";
const char* apPassword = "123456789";

AsyncWebServer server(80);

void setup() {
  Serial.begin(9600);
  Serial.println("Hello");
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
  }

  JsonArrayConst arr = getNetworks();
  for(JsonVariantConst v : arr){
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