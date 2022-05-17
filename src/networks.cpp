#include "networks.hpp"

bool loadedFile = false;
StaticJsonDocument<1024> wifiNetworks;

std::string readFile(const char* filename){
  File f = SPIFFS.open(filename);
  if(!f){
    Serial.print("Failed to open file: ");
    Serial.println(filename);
    return "";
  }
  std::string buffer;
  while(f.available()){
    buffer+=static_cast<char>(f.read());
  }
  f.close();
  return buffer;
}

void loadNetworkFile(){
  std::string buffer = readFile("/WiFi_Credentials.json");
  DeserializationError error = deserializeJson(wifiNetworks, buffer);
  if(error){
    Serial.println("Deserializing WiFi_Credentials.json failed");
    Serial.println(error.f_str());
    return;
  }
}

JsonArrayConst getNetworks(){
  if(!loadedFile){
    loadNetworkFile();
  }
  return wifiNetworks.as<JsonArray>();
}

// returns true if the operation was successfull
// bool addNetwork(char* SSID, char* PW){
    
// }