// 10/02/2024
// Last Update ?? Version definitive

#include <Arduino.h>

#include "ledController.h"
#include "wifi_2024.h"
#include "interruption.h"
#include "websocket.h"
//#include "websocket.cpp"

#include <WiFi.h>
#include <SPIFFS.h>

struct LedParam LedGD1, *PLedGD1=&LedGD1;
struct LedParam LedGD2, *PLedGD2=&LedGD2;

hw_timer_t *myTimer0=NULL;

// *********** SETUP **********
void setup() {
  Serial.begin(115200);

  pinMode(IO_Door1, INPUT);
  pinMode(IO_Door2, INPUT);
  pinMode(IO_LDR, INPUT);
  pinMode(IO_Home, INPUT);
  
  pinMode(IO_Relay ,OUTPUT);
  pinMode(IO_GD1_Status ,OUTPUT);
  pinMode(IO_GD2_Status ,OUTPUT);

  digitalWrite(IO_Relay, LOW);
  digitalWrite(IO_GD1_Status, HIGH);
  digitalWrite(IO_GD2_Status, HIGH);

  // Leds Setup
  SetLedParam(PLedGD1, BLINK, 150, 500);
  SetLedParam(PLedGD2, BLINK, 150, 1000);

  // Timer Setup
  myTimer0 = timerBegin(0, 80, true);
  timerAttachInterrupt(myTimer0, &onmyTimer0Overflow, true);
  timerAlarmWrite(myTimer0, 50000, true);  // Interruption toutes les 50000us soit 50ms
  timerAlarmEnable(myTimer0);

  // Interruption Setup
  attachInterrupt(digitalPinToInterrupt(IO_Door1), DG1_SetRelay, RISING);
  attachInterrupt(digitalPinToInterrupt(IO_Door1), DG1_SetRelay, FALLING);

  attachInterrupt(digitalPinToInterrupt(IO_Door2), DG2_SetRelay, RISING); 
  attachInterrupt(digitalPinToInterrupt(IO_Door2), DG2_SetRelay, FALLING);

  // Initialize Wifi
  const char* ssid = "Freebox-372EBF";
  const char* password = "mfrfzq7db9q43xzrqmv49b";

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED){
    Serial.println("WiFi Failed!");
    return;
  }

  Serial.println();
  Serial.print("ESP IP Address: http://");
  Serial.println(WiFi.localIP());

  // Initialize SPIFFS
  if(!SPIFFS.begin())
    Serial.println("An Error has occured while mounting SPIFFS");
    else
    Serial.println("SPIFFS Mounted successfully");
      
  initWebSocket();
}


// ********* MAIN PROGRAMM **********
void loop() {
//  TimeManager();
//  SetDayState();
//  SetAliveLed();
//  CheckDoor();
//  setRelayOff();
//  ws.cleanupClients();
}
