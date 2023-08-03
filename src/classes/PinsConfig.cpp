#include "PinsConfig.h"

#include "../../include/constants.h"

PinsConfig::PinsConfig() {

}

void PinsConfig::applyChange() {
  boolean vsin, r1, c, l, r2;

  vsin = (boolean) pinsConfig[0];
  r1   = (boolean) pinsConfig[1];
  c    = (boolean) pinsConfig[2];
  l    = (boolean) pinsConfig[3];
  r2   = (boolean) pinsConfig[4];

  // vsin = (boolean) configDoc["rele1"];
  // r1   = (boolean) configDoc["rele2"];
  // c    = (boolean) configDoc["rele3"];
  // l    = (boolean) configDoc["rele4"];
  // r2   = (boolean) configDoc["rele5"];

  if (vsin) {
    // Desconectar el circuito y esperar
    digitalWrite(PIN_WRITE_VSIN, LOW);
    delay(DELAY_RELES);

    // Realizar los cambios en el circuito
    digitalWrite(PIN_WRITE_R1, r1);
    digitalWrite(PIN_WRITE_R2, r2);
    // digitalWrite(PIN_WRITE_C, c);
    // digitalWrite(PIN_WRITE_L, l);

    // Esperar y encender el circuito
    delay(DELAY_RELES);
    digitalWrite(PIN_WRITE_VSIN, HIGH);
  }
  else {
    digitalWrite(PIN_WRITE_VSIN, LOW);
  }
}


// @todo: Adaptar funcion para aceptar json's
void PinsConfig::setConfig(String jsonString) {
  USE_SERIAL.print("jsonString:");
  USE_SERIAL.println(jsonString);

  for (int x = 0; x < 5; x++) {
    int dotPos = jsonString.indexOf(":");
    int separatorPos = (jsonString.indexOf(",") != -1) ? jsonString.indexOf(",") : jsonString.indexOf("}");
    
    if ((separatorPos - dotPos) == 6)
      pinsConfig[x] = 0;
    else if ((separatorPos - dotPos) == 5)
      pinsConfig[x] = 1;
      
    jsonString = jsonString.substring(separatorPos+1);
  }
  
  Serial.println("New config:");
  for (int x = 0; x < 5; x++) {
      Serial.printf("pinsConfig %d: %d\n", x , pinsConfig[x]);
  }

  this->applyChange();

  // DeserializationError err = deserializeJson(this->configDoc, jsonString.c_str());
  
  // if (!err) {
  //   this->applyChange();
  // }
}
