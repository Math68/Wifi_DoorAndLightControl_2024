// 16/02/2024
// Last Update ?? Version definitive

#include <Arduino.h>
#include <Arduino_JSON.h>
#include "ledController.h"
#include "interruption.h"
#include "websocket.h"
//#include "websocket.cpp"

#include <WiFi.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define DAY 1
#define NIGHT 0

#define CLOSED 0
#define OPEN 1

// *************** SET VARIABLES ****************

bool RelaisOn;
int PreviousTime=0;
int PreviousDebounceTime=0;
int CurrentDebounceTime=0;

struct LedParam LedDoorG1;
struct LedParam LedDoorG2;

LedParam *PLedDoorG1 = &LedDoorG1;
LedParam *PLedDoorG2 = &LedDoorG2;

//hw_timer_t *myTimer=NULL;

extern Actions Action;

String DoorMathState="Closed";
String DoorCaroState="Closed";

/*
// Json Variable
JSONVar DoorsState;

String doorsState(){
  
  if(digitalRead(IO_Door1))
    DoorMathState = "Open";
    else
    DoorMathState = "Closed";
  
  if(digitalRead(IO_Door2))
    DoorCaroState = "Open";
    else
    DoorCaroState = "Closed";
  
  DoorsState["DoorMathState"]=String(DoorMathState);
  DoorsState["DoorCaroState"]=String(DoorCaroState);

  String jsonString = JSON.stringify(DoorsState);
  return jsonString;
}
*/

// ************** SET PIN NUMBERS ***************
// 34 -> 39 only inputs
// Inputs
const int IO_DoorG1 = 26;
const int IO_DoorG2 = 14;
const int IO_LDR = 15;   // ADC1_CH
const int IO_Home = 12;

// Outputs
const int IO_Relay = 25;
const int IO_LedDoorG1 = 19;
const int IO_LedDoorG2 = 17;

// *********** FUNCTIONS DRINITION *********

void setLedState();

// **********************************************
// ***************** SETUP **********************
// **********************************************

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
  SetLedParam(PLedDoorG2, BLINK, 150, 2000);

/*
  // Timer Setup
  myTimer = timerBegin(1, 16000, true);                        // Interruption all 200 us
  timerAttachInterrupt(myTimer, &onmyTimerOverflow, true);
  timerAlarmWrite(myTimer, 5000, true);                        // Interruption after 1s
//  timerAlarmEnable(myTimer);
*/
  // Interruption Setup
  attachInterrupt(IO_DoorG1, &onDoorG1StateChanged, HIGH);
  
  attachInterrupt(IO_DoorG2, &onDoorG2StateChanged, RISING);
  
  attachInterrupt(IO_LDR, &onDayStateChanged, CHANGE); 

  // Initialize Wifi
//  const char* ssid = 
//  const char* password =
/*
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED){
    Serial.println("WiFi Failed!");
    return;
  }

  Serial.println();
  Serial.print("ESP IP Address: http://");
  Serial.println(WiFi.localIP());
*/
/*
  // Initialize SPIFFS
  if(!SPIFFS.begin())
    Serial.println("An Error has occured while mounting SPIFFS");
    else
    Serial.println("SPIFFS Mounted successfully");
  */    
  initWebSocket();

  //PreviousTime = millis();
  Serial.println("Setup complete");

  Action=SOLVED;
}

// ********* MAIN PROGRAMM **********
void loop() {
  GPIOController(PLedDoorG1, IO_LedDoorG1);
  GPIOController(PLedDoorG2, IO_LedDoorG2);

  switch(Action){
    case DAYSTATECHANGED:{
      Action=SOLVED;
      Serial.println("Action: Day State changed");
      setLedState();
      break;
    }
    case DOORG1STATECHANGED:{
      Serial.println("Debounce Time: " + String(millis()));
      if( millis()-PreviousDebounceTime >30){
        if(digitalRead(IO_DoorG1)==HIGH)
        {
          setLedState();
          digitalWrite(IO_Relay, HIGH);
          PreviousTime=millis();
          RelaisOn=true;
        }
        Action=SOLVED;
      }
      break;
    }
    case DOORG2STATECHANGED:{
      Action=SOLVED;
      break;
    }
    case MYTIMERHASOVERFLOW:{
      Action=SOLVED;
      break;
    }
    case SOLVED:{
      break;
    }
    case DEBOUNCEG1:{
      PreviousDebounceTime = millis();
      Action=DOORG1STATECHANGED;
      break;
    }
    case DEBOUNCEG2:{
      PreviousDebounceTime = millis();
      Action=DOORG2STATECHANGED;
      break;
    }
  }

  if(RelaisOn==true)  
  {
    if((millis()-PreviousTime)>1000){
      RelaisOn=false;
      digitalWrite(IO_Relay, LOW);
      Serial.println("Relais Closed " + String(PreviousTime));
    }
  }

}
  //ws.cleanupClients();

// ***************** FUNCTIONS ******************

void setLedState(){
  if(digitalRead(IO_LDR) == NIGHT){

    Serial.println("It's Night \n");

    if(digitalRead(IO_DoorG1) == OPEN){
      SetLedParam(PLedDoorG1, FLASH_ONE_INV, 150, 2000);
    }
    else
    SetLedParam(PLedDoorG1, FLASH_ONE, 150, 2000);

    if(digitalRead(IO_DoorG2) == OPEN){
      SetLedParam(PLedDoorG2, FLASH_TWO_INV, 150, 2000);
    }
    else
    SetLedParam(PLedDoorG2, FLASH_TWO, 150, 2000);

  }
  
  if(digitalRead(IO_LDR) == DAY){   // if it's Day

    Serial.println("It's Day \n");

    if(digitalRead(IO_DoorG1) == OPEN){
      SetLedParam(PLedDoorG1, FLASH_ONE_INV, 100, 1000);
    }
    else
    SetLedParam(PLedDoorG1, FLASH_ONE, 100, 1000);

    if(digitalRead(IO_DoorG2) == OPEN){
      SetLedParam(PLedDoorG2, FLASH_TWO_INV, 100, 1000);
    }
    else
    SetLedParam(PLedDoorG2, FLASH_TWO, 100, 1000);
  }
}