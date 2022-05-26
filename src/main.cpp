#include <Arduino.h>
#include <Preferences.h>
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"

const char* apSSID = "ESP-Epaper";
const char* apPassword = "123456789";
Preferences preferences;

AsyncWebServer server(80);

void setup() {
  Serial.begin(9600);
  preferences.begin("wifi");
  if(!preferences.isKey("ssid")){ // initializes keys during first run
    preferences.putString("ssid","");
    preferences.putString("password","");
  }

  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
  }

  WiFi.disconnect();
  String ssid = preferences.getString("ssid","");
  String password = preferences.getString("password", "");
  Serial.print("Stored SSID is: ");
  Serial.println(ssid);

  if(ssid!=""){ // try to connect to known Access Point
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(),password.c_str());
    for(int i=0;i<10 || WiFi.status() != WL_CONNECTED;++i){
      Serial.print('.');
      delay(1000);
    }
  }
  if(!WiFi.isConnected()){ // Setup Hotspot if no WiFi connection is possible
    Serial.println("Failed to connect to WiFi. Starting Access Point");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(apSSID, apPassword);
  }
  else{
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  }
  
  server.on("/setup", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/setup.html", "text/html");
  });

  server.on("/setup", HTTP_POST, [](AsyncWebServerRequest *request){
    Serial.println("Received Post");
    int params = request->params();
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
          Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
        else{
          Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
      }
      if(request->hasParam("ssid", true)){
        AsyncWebParameter* p = request->getParam("ssid", true);
        Serial.print("Saving SSID: ");
        Serial.println(p->value());
        preferences.putString("ssid",p->value());
      }
      if(request->hasParam("password",true)){
        AsyncWebParameter* p = request->getParam("password", true);
        Serial.print("Saving password: ");
        Serial.println(p->value());
        preferences.putString("password",p->value());
      }
      if(request->hasParam("clearsettings",true)){
        Serial.println("Clearing settings");
        preferences.clear();
      }
      ESP.restart();
  });
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}