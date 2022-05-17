#include <string>
#include "ArduinoJson.h"
#include "SPIFFS.h"

std::string readfile(const char* filename);
JsonArrayConst getNetworks();
void addNetwork(char* SSID, char* PW);