
#include <Arduino.h>
#include "websocket.h"
#include <AsyncTCP.h>
#include <SPIFFS.h>

#include <ledController.h>

// Replace with your network credentials
const char* ssid = "Freebox-372EBF";
const char* password = "mfrfzq7db9q43xzrqmv49b";

//const char* ssid = "EMCP-Guest";
//const char* password = "92*Shir#kh@n!07";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
CabinetWebsocket websocket(&server, &ws);


void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}