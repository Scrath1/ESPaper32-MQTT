#include <Arduino.h>
#include <Preferences.h>

#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "webserver.h"
#include "epaper/epaper.h"

const char* apSSID = "ESP-Epaper";
const char* apPassword = "123456789";
Preferences preferences;

AsyncWebServer server(80);

void setup() {
    Serial.begin(9600);
    preferences.begin("pref");
    if (!preferences.isKey("ssid")) {  // initializes keys during first run
        preferences.putString("ssid", "");
        preferences.putString("password", "");
    }

    WiFi.disconnect();
    String ssid = preferences.getString("ssid", "");
    String password = preferences.getString("password", "");
    Serial.print("Stored SSID is: ");
    Serial.println(ssid);

    if (ssid != "") {  // try to connect to known Access Point
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());
        for (int i = 0; i < 10 || WiFi.status() != WL_CONNECTED; ++i) {
            Serial.print('.');
            delay(1000);
        }
    }
    if (!WiFi.isConnected()) {  // Setup Hotspot if no WiFi connection is possible
        Serial.println("Failed to connect to WiFi. Starting Access Point");
        WiFi.mode(WIFI_AP);
        WiFi.softAP(apSSID, apPassword);
    } else {
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
    }
    setupWebServer(server);
    displayTest();
}

void loop() {
    // put your main code here, to run repeatedly:
}