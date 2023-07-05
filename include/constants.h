#pragma once
/*
* CONSTANTES
*/

#define USE_SERIAL Serial

// Configuraciones del programa
#define CANTIDAD_MUESTRAS 500
#define MESSAGE_DELAY 500
#define DELAY_RELES 2000

// Pines de salida
#define PIN_WRITE_VISN 23
#define PIN_WRITE_R1 22
#define PIN_WRITE_R2 2
#define PIN_WRITE_C 5
#define PIN_WRITE_L 4

// Pines de lectura
#define PIN_READ_VOLT 33
#define PIN_READ_CURR 32

// Parametros Pantalla OLED
#ifdef COMPILE_DISPLAY
  #define DIR_I2C 0x3c   //Direccion Display Oled HEX
  #define LED_ANCHO 128  // OLED display width, in pixels
  #define LED_ALTO 64    // OLED display height, in pixels
  #define OLED_RESET -1  // Reset pin # (or -1 if sharing reset pin)
#endif

#define WIFI_SSID "FliaBarrera"
#define WIFI_PASSWORD "heladera24"
#define HOST_IP "192.168.100.161"
#define HOST_PORT 3000
#define HOST_AUTHORIZATION "Bearer token"