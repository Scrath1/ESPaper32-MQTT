#include "ArduinoJson.h"

bool addedNetwork = false;

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

