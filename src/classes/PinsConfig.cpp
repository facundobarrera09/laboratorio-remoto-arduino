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

  vsin = (boolean) configDoc[0];
  r1   = (boolean) configDoc[1];
  c    = (boolean) configDoc[2];
  l    = (boolean) configDoc[3];
  r2   = (boolean) configDoc[4];

  // vsin = (boolean) configDoc["rele1"];
  // r1   = (boolean) configDoc["rele2"];
  // c    = (boolean) configDoc["rele3"];
  // l    = (boolean) configDoc["rele4"];
  // r2   = (boolean) configDoc["rele5"];

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
