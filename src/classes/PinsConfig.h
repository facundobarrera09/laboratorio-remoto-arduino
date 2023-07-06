#pragma once

#include <ArduinoJson.h>

class PinsConfig {
private:
  StaticJsonDocument<JSON_OBJECT_SIZE(24)> configDoc;

  int pinsConfig[5] = {0,0,0,0,0};
  void applyChange();

public:
  PinsConfig();
  void setConfig(String jsonString);
  // DeserializationError setConfig(String jsonString);
};