#define DEBUG true

#define COMPILE_DISPLAY
#define COMPILE_SOCKET
#define COMPILE_JSON
#define SIMULAR_VALORES true

#include "setup.h"
#include "constants.h"
#include "bitmaps.h"

// Funciones de simulacion de datos
int simularVoltaje(int x);
int simularCorriente(int x);

// void checkHeap() {
//   Serial.print("heap: ");
//   Serial.println(ESP.getFreeHeap());
// }

void connectedToIO(const char* payload, size_t length) {
  Serial.printf("connected to webSocket\n");
  estado_conexion = 1;
}

void setReceivedConfig(const char* payload, size_t length) {
  Serial.println("Received config");
  USE_SERIAL.println("PAYLOAD:");
  USE_SERIAL.println(payload);
  estado_conexion = 2;

  DeserializationError err = config.setConfig(payload);
  
  if (err) {
    USE_SERIAL.println("Error deserializing JSON config string");
    String error = String("{ \"error\": \"") + err.f_str() + String("\"}");
    webSocket.emit("config:error", error.c_str());
    estado_conexion = 1;

    webSocket.loop();

    while (true);
  }
  else {
    estado_conexion = 3;
  }
}

void setup() {
  USE_SERIAL.begin(115200);

  USE_SERIAL.setDebugOutput(DEBUG);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  #ifdef COMPILE_DISPLAY
  if (!display.begin(DIR_I2C, true)) {
    Serial.println(F("Pantalla OLED No Encontrada"));
    for (;;)
      ;  //Eterno bucle sin hacer nada xD
  }
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);                        //Define el color de las fuentes
  display.drawBitmap(0, 0, epd_bitmap_logoFRT, 128, 64, 1);  //Muestra el logo en forma de campo de bits
  display.display();
  delay(3000);
  display.clearDisplay();
  #endif

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("ciasur", "2022ciasur");

  webSocket.on("connect", connectedToIO);
  webSocket.on("config:set", setReceivedConfig);
  webSocket.begin("192.168.60.119", 3000, "/socket.io/?transport=websocket&type=esp");
  webSocket.setAuthorization("facu", "barrera");

  // pinMode(rele_vsin, OUTPUT);
  // pinMode(rele_r1, OUTPUT);
  // pinMode(rele_c, OUTPUT);
  // pinMode(rele_l, OUTPUT);
  // pinMode(rele_r2, OUTPUT);
  // pinMode(read_1, INPUT);
  // pinMode(read_2, INPUT);
}

void loop() {
  if (WiFiMulti.run() == WL_CONNECTED) {
    if (estado_conexion == 1) {
      webSocket.emit("config:get", "{}");
      delay(MESSAGE_DELAY);
    }

    if (estado_conexion == 2) {
      //
    }

    if (estado_conexion == 3) {
      int voltaje[CANTIDAD_MUESTRAS] = {};
      int corriente[CANTIDAD_MUESTRAS] = {};

      for (int x = 0; x < CANTIDAD_MUESTRAS; x++) {
        if (SIMULAR_VALORES) {
          voltaje[x] = simularVoltaje(x);
          corriente[x] = simularCorriente(x);
        } else {
          voltaje[x] = analogRead(PIN_READ_VOLT);
          corriente[x] = analogRead(PIN_READ_CURR);
        }
      }

      String data = "{\"voltage\": [";

      for (int x = 0; x < CANTIDAD_MUESTRAS; x++) {
        if (x != 0) data += ",";
        data += String(voltaje[x]);
      }

      data += "], \"current\": [";
      for (int x = 0; x < CANTIDAD_MUESTRAS; x++) {
        if (x != 0) data += ",";
        data += String(corriente[x]);
      }
      data += "]}";

      webSocket.emit("meassurement_data:post", data.c_str());
      estado_conexion = 0;
    }

    webSocket.loop();
  }
}


int simularVoltaje(int x) {
  return 100 * sin(((2 * PI * x) / 100) + (PI * 1.2));
}

int simularCorriente(int x) {
  return 100 * sin((2 * PI * x) / 100);
}