#define DEBUG false

// #define COMPILE_DISPLAY
#define COMPILE_SOCKET
#define COMPILE_JSON
// #define COMPILE_CONSOLE

#ifdef COMPILE_CONSOLE
  #include <ESP32Console.h>
  using namespace ESP32Console;
  Console console;

  #define PRINTF(...) printf(__VA_ARGS__);
#else
  #define PRINTF(...) Serial.printf(__VA_ARGS__); Serial.flush();
#endif

#include "setup.h"
#include "constants.h"
#include "bitmaps.h"

// Funciones de simulacion de datos
bool simular_valores = true;
int simularVoltaje(int x);
int simularCorriente(int x);

// void checkHeap() {
//   Serial.print("heap: ");
//   Serial.println(ESP.getFreeHeap());
// }

void connectedToIO(const char* payload, size_t length) {
  PRINTF("connected to webSocket\n");
  estado_conexion = 1;
}

void connectionError(const char* payload, size_t length) {
  if (String(payload) == "unauthorized") {
    PRINTF("Connection refused: unauthorized");
    PRINTF("- Use the serial console to set the proper auth value and to attempt connection again (SetAuthorization)");

    estado_conexion = -1;
  }
}

void setReceivedConfig(const char* payload, size_t length) {
  PRINTF("Received config");
  PRINTF("PAYLOAD:");
  PRINTF(payload);
  estado_conexion = 2;

  DeserializationError err = config.setConfig(payload);
  
  if (err) {
    PRINTF("Error deserializing JSON config string");
    String error = String("{ \"error\": \"") + err.f_str() + String("\"}");
    webSocket.emit("config:error", error.c_str());
    estado_conexion = 1;

    webSocket.loop();

    while (true) {};
  }
  else {
    estado_conexion = 3;
  }
}

#ifdef COMPILE_CONSOLE
int socketCommand(int argc, char **argv) {
  if (argc < 2) {
    PRINTF("Usage:\n\t- socket.io [options]\n\nOptions:\n\t --set-authorization VALUE : Set the authorization value to be sent during the handshake\n\t -r : Restart socket connection");
    
    return EXIT_FAILURE;
  }
  
  for (int x = 1; x < argc; x++) {
    auto arg = String(argv[x]);

    if (arg == "--list-config") {
      if (x == 1) {
        PRINTF("Configuracion de socket.io\n");
        PRINTF("\t- estado_conexion    = %d\n", estado_conexion);
        PRINTF("\t- host_ip            = %s\n", host_ip);
        PRINTF("\t- host_port          = %d\n", host_port);
        PRINTF("\t- host_authorization = %s\n", host_authorization);
      }
    }

    if (arg == "--set-authorization") {
      if (++x == argc) return EXIT_FAILURE;
      
      host_authorization = String(argv[x]);
      PRINTF("Changed authorization string to %s\n", host_authorization);
    }

    if (arg == "--set-simulation") {
      if (++x == argc) return EXIT_FAILURE;

      simular_valores = (String(argv[x]) == "true") ? true : false;
    }

    if (arg == "-r") {
      PRINTF("Disconnecting from socket\n");
      webSocket.disconnect();
      estado_conexion = 0;
    }
  }
  
  return EXIT_SUCCESS;
}
#endif

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(DEBUG);

  PRINTF("\n\n\n");
  for (uint8_t t = 4; t > 0; t--) {
    PRINTF("[SETUP] BOOT WAIT %d...\n", t);
    delay(1000);
  }

  #ifdef COMPILE_DISPLAY
  if (!display.begin(DIR_I2C, true)) {
    PRINTF(F("Pantalla OLED No Encontrada"));
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

  #ifdef COMPILE_SOCKET
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  webSocket.on("connect", connectedToIO);
  webSocket.on("connection:error", connectionError);
  webSocket.on("config:set", setReceivedConfig);

  strcpy(host_ip, HOST_IP);
  host_port = HOST_PORT;
  host_authorization = HOST_AUTHORIZATION;
  #endif

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

    webSocket.begin(host_ip, host_port, "/socket.io/?transport=websocket&type=esp");
    webSocket.setAuthorization(host_authorization.c_str());

    while (WiFiMulti.run() == WL_CONNECTED && estado_conexion != 0) {
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
          if (simular_valores) {
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

      if (estado_conexion == -1) {
        USE_SERIAL.println("bloqueando esp");
        while (Serial.available() < 1) {};
      }

      webSocket.loop();
    }
  }
}


int simularVoltaje(int x) {
  return 100 * sin(((2 * PI * x) / 100) + (PI * 1.2));
}

int simularCorriente(int x) {
  return 100 * sin((2 * PI * x) / 100);
}