// 10/02/2024
// Last Update ?? Version definitive

#include <Arduino.h>

#include "ledController.h"
#include "wifi_2024.h"
#include "interruption.h"
#include "websocket.h"
#include "websocket.cpp"

#include <WiFi.h>
#include <SPIFFS.h>

Actions Action=SOLVED;

//bool myDayStateChanged=false;
//bool DoorG1_Status=CLOSED;
//bool DoorG2_Status=CLOSED;
//bool PinStatus=0;

bool currentDoorG1State=CLOSED;
bool currentDoorG2State=CLOSED;
bool currentDayState=NIGHT;
bool dayState;

int PreviousTime;

struct LedParam LedDoorG1;
struct LedParam LedDoorG2;

LedParam *PLedDoorG1 = &LedDoorG1;
LedParam *PLedDoorG2 = &LedDoorG2;

hw_timer_t *myTimer0=NULL;
hw_timer_t *myTimer1=NULL;

// *********** SETUP **********
void setup() {
  Serial.begin(115200);
  Serial.println("Setup Sart");

  pinMode(IO_DoorG1, INPUT);
  pinMode(IO_DoorG2, INPUT);
  pinMode(IO_LDR, INPUT);
  pinMode(IO_Home, INPUT);
  
  pinMode(IO_Relay ,OUTPUT);
  pinMode(IO_LedDoorG1, OUTPUT);
  pinMode(IO_LedDoorG2,OUTPUT);

  digitalWrite(IO_Relay, LOW);
  digitalWrite(IO_LedDoorG1, HIGH);
  digitalWrite(IO_LedDoorG2, HIGH);

  // Set Leds
  SetLedParam(PLedDoorG1, FLASH_THREE, 100, 1500);
//  SetLedParam(PLedDoorG2, BLINK, 150, 2000);

  // Timer Setup
  myTimer0 = timerBegin(0, 80, true);
  timerAttachInterrupt(myTimer0, &onmyTimer0Overflow, true);
  timerAlarmWrite(myTimer0, 50000, true);  // Interruption toutes les 50000us soit 50ms
  timerAlarmEnable(myTimer0);

  myTimer1 = timerBegin(0, 80, true);
  timerAttachInterrupt(myTimer1, &onmyTimer1Overflow, true);
  timerAlarmWrite(myTimer1, 50000, true);  // Interruption toutes les 50000us soit 50ms
  timerAlarmEnable(myTimer1);

  // Interruption Setup
  attachInterrupt(digitalPinToInterrupt(IO_DoorG1), onDoorG1Opened, RISING);
  attachInterrupt(digitalPinToInterrupt(IO_DoorG1), onDoorG1Closed, FALLING);
  
  attachInterrupt(digitalPinToInterrupt(IO_DoorG2), onDoorG2Opened, RISING);
  attachInterrupt(digitalPinToInterrupt(IO_DoorG2), onDoorG2Closed, FALLING);
  
  attachInterrupt(digitalPinToInterrupt(IO_LDR), onDayStateChanged, CHANGE); 

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

  PreviousTime = millis();
  Serial.println("Setup complete");
}


// ********* MAIN PROGRAMM **********
void loop() {
  GPIOController(PLedDoorG1, IO_LedDoorG1);
  GPIOController(PLedDoorG2, IO_LedDoorG2);

  switch(Action){
    case DAYSTATECHANGED:{
      Action=SOLVED;
      setLedState();

    };
    case DOORG1OPENED:{
      Action=SOLVED;

    };
    case DOORG1CLOSED:{
      Action=SOLVED;

    };
    case DOOG2OPENED:{
      Action=SOLVED;

    };
    case DOORG2CLOSED:{
      Action=SOLVED;

    };
    case SOLVED:{
      Serial.println("Action Solved " + String(millis()));
    }
  }
  ws.cleanupClients();
}
/*
  if(millis() - PreviousTime > 5000)     // all 5s
  {
    Serial.println("Passage dans boucle " + String(PreviousTime));
    PreviousTime=millis();
    
    dayState=getDayState();

    Serial.println("Day State " + String(dayState));

    if(dayState != currentDayState)
    {
      currentDayState=dayState;
      Serial.println("Current Day State" + String(currentDayState));
      if(dayState == DAY){
        Serial.println("Set Output to High");
        //digitalWrite(IO_LedDoorG2, LOW);
        SetLedParam(PLedDoorG2, FLASH_TWO, 100, 1000);
      }
      else{
        Serial.println("Set Output to Low");
        //digitalWrite(IO_LedDoorG2, HIGH);
        SetLedParam(PLedDoorG2, FLASH_TWO, 100, 2000);
      }
    }
  }
*/


