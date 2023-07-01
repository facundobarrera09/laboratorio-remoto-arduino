#pragma once

#include <ArduinoJson.h>

class PinsConfig {
private:
  StaticJsonDocument<JSON_OBJECT_SIZE(24)> configDoc;
  void applyChange();

public:
  PinsConfig();
  DeserializationError setConfig(String jsonString);
};