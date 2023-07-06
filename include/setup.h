#pragma once

/*
* LIBRERIAS
*
* - Se requiere socket.io-client-master (descargar directo de github)
*
*/

// #include <Arduino.h>

#ifdef COMPILE_DISPLAY
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SH110X.h>
#endif

#ifdef COMPILE_SOCKET
  #include <SocketIoClient.h>
  #include <WiFi.h>
  #include <WiFiMulti.h>
#endif

#ifdef COMPILE_JSON
  // #include <ArduinoJson.h>
  #include <ArduinoJson.h>
  #include "../src/classes/PinsConfig.h"
#endif

// #include <Arduino.h>

/*
* CONFIGURACIONES
*/
#include "constants.h"

// Conexion
int estado_conexion = 0;  // -1 - error: esperando accion del usuario, 0 - desconectado, 1 - esperando mensaje del servidor, 2 - procesando mensaje del servidor, 3 - enviando mediciones al servidor

char host_ip[16];
int host_port;
String host_authorization;

#ifdef COMPILE_SOCKET
  WiFiMulti WiFiMulti;
  SocketIoClient webSocket;

  char wifiSSID[33] = "FliaBarrera";
  char wifiPassword[64] = "heladera24";

  char hostIp[] = "192.168.100.161";
  int hostPort = 3000;
  char hostAuhtorizationType[] = "Bearer";
  char hostAuthorization[100] = "valid token";
#endif

// Pins
#ifdef COMPILE_JSON
  PinsConfig config;
#endif

// Display
#ifdef COMPILE_DISPLAY
  Adafruit_SH1106G display = Adafruit_SH1106G(LED_ANCHO, LED_ALTO, &Wire, OLED_RESET);
#endif

