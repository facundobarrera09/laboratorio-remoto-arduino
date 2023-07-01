#include "PinsConfig.h"

#include "../../include/constants.h"

#define RELE_VSIN 0
#define RELE_R1 1
#define RELE_R2 2
#define RELE_C 3
#define RELE_L 4

PinsConfig::PinsConfig() {

}

void PinsConfig::applyChange() {
  boolean vsin, r1, c, l, r2;

  vsin = (boolean) configDoc["rele1"];
  r1   = (boolean) configDoc["rele2"];
  c    = (boolean) configDoc["rele3"];
  l    = (boolean) configDoc["rele4"];
  r2   = (boolean) configDoc["rele5"];

  if (vsin) {
    // Desconectar el circuito y esperar
    digitalWrite(RELE_VSIN, LOW);
    delay(DELAY_RELES);

    // Realizar los cambios en el circuito
    digitalWrite(RELE_R1, r1);
    digitalWrite(RELE_R2, r2);
    digitalWrite(RELE_C, c);
    digitalWrite(RELE_L, l);

    // Esperar y encender el circuito
    delay(DELAY_RELES);
    digitalWrite(RELE_VSIN, HIGH);
  }
  else {
    digitalWrite(RELE_VSIN, LOW);
  }
}

DeserializationError PinsConfig::setConfig(String jsonString) {
  DeserializationError err = deserializeJson(configDoc, jsonString);
  
  if (!err) {
    this->applyChange();
  }

  return err;
}
