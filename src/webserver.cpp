#include "webserver.h"

extern Preferences preferences;

void listParameters(AsyncWebServerRequest* request) {
    int params = request->params();
    for (int i = 0; i < params; i++) {
        AsyncWebParameter* p = request->getParam(i);
        if (p->isPost()) {
            Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        } else {
            Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
    }
}

void setupRequests(AsyncWebServer& server) {
    if (!SPIFFS.begin()) {
        Serial.println("An Error has occurred while mounting SPIFFS");
    }

    server.on("/setup", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(SPIFFS, "/setup.html", "text/html");
    });

    server.on("/setup", HTTP_POST, [](AsyncWebServerRequest* request) {
        listParameters(request);
        if (request->hasParam("ssid", true)) {
            AsyncWebParameter* p = request->getParam("ssid", true);
            Serial.println("Saving WiFi SSID");
            preferences.putString("ssid", p->value());
        }
        if (request->hasParam("password", true)) {
            AsyncWebParameter* p = request->getParam("password", true);
            Serial.println("Saving WiFi password");
            preferences.putString("password", p->value());
        }
        if (request->hasParam("clearsettings", true)) {
            Serial.println("Clearing settings");
            preferences.clear();
        }
        ESP.restart();
    });
}

void setupWebServer(AsyncWebServer& server) {
    setupRequests(server);
    server.begin();
}