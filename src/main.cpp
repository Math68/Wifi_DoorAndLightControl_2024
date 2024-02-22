// 22/02/2024 6H31
// Last Update ?? Version definitive

#include <Arduino.h>
#include <Arduino_JSON.h>
#include "ledController.h"
#include "interruption.h"
#include "websocket.h"
#include "password.h"
//#include "websocket.cpp"

#include <WiFi.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define DAY 1
#define NIGHT 0
#define CLOSED 0
#define OPEN 1
#define ISR_Solved ISR_Event=SOLVED;

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

// *************** SET VARIABLES ****************

bool RelaisOn;
int RelaisOnTime=0;
int DebounceTime=0;

struct LedParam LedDoorG1;
struct LedParam LedDoorG2;

LedParam *PLedDoorG1 = &LedDoorG1;
LedParam *PLedDoorG2 = &LedDoorG2;

extern ISR_Events ISR_Event;

// ***** Json Variable *****

String DoorMathState="Closed";
String DoorCaroState="Closed";

JSONVar DoorsState;

String doorsState(){
  
  if(digitalRead(IO_DoorG1))
    DoorMathState = "Open";
    else
    DoorMathState = "Closed";
  
  if(digitalRead(IO_DoorG2))
    DoorCaroState = "Open";
    else
    DoorCaroState = "Closed";
  
  DoorsState["DoorMathState"]=String(DoorMathState);
  DoorsState["DoorCaroState"]=String(DoorCaroState);

  String jsonString = JSON.stringify(DoorsState);
  return jsonString;
}

// *********** FUNCTIONS DRINITION *********
void setLedState();
void setRelaisOn();
void setRelaisOff();
bool getDoorG1();
bool getDoorG2();
bool getDayState();

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

  // Interruption Setup
  attachInterrupt(IO_DoorG1, &ISR_DoorG1Moved, CHANGE);
  attachInterrupt(IO_DoorG2, &ISR_DoorG2Moved, CHANGE);  
  attachInterrupt(IO_LDR, &ISR_DayStateChanged, CHANGE); 

  // Initialize Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED){
    Serial.println("WiFi Failed!");
    return;
  }

  Serial.println();
  Serial.print("ESP IP Address: http://");
  Serial.println(WiFi.localIP());

/*
  // Initialize SPIFFS
  if(!SPIFFS.begin())
    Serial.println("An Error has occured while mounting SPIFFS");
    else
    Serial.println("SPIFFS Mounted successfully");
  */    
  initWebSocket();
  ISR_Solved;
}

// ********* MAIN PROGRAMM **********
void loop() {

  GPIOController(PLedDoorG1, IO_LedDoorG1);
  GPIOController(PLedDoorG2, IO_LedDoorG2);

  if(RelaisOn==true)  
  {
    if((millis()-RelaisOnTime)>1000){
      setRelaisOff();
    }
  }

  switch(ISR_Event){
    case DAYSTATECHANGED:{
      ISR_Solved;
      setLedState();
      break;
    }
    case DOORG1DEBOUNCE:{
      DebounceTime = millis();
      ISR_Event=DOORG1MOVED;
      break;
    }
    case DOORG2DEBOUNCE:{
      DebounceTime = millis();
      ISR_Event=DOORG2MOVED;
      break;
    }
    case DOORG1MOVED:{      
      if( millis()-DebounceTime >10){
        if(getDoorG1()==HIGH)
        {
          setLedState();
          if(getDayState()==NIGHT){
            setRelaisOn();
          }
        }
        ISR_Solved;
      }
      break;
    }
    case DOORG2MOVED:{
      ISR_Solved
      break;
    }
    case SOLVED:{
      break;
    }
  }
}
  //ws.cleanupClients();

// ***************** FUNCTIONS ******************

void setLedState(){
  if(getDayState() == NIGHT){

    Serial.println("It's Night \n");

    if(getDoorG1() == OPEN){
      SetLedParam(PLedDoorG1, FLASH_ONE_INV, 150, 2000);
    }
    else
    SetLedParam(PLedDoorG1, FLASH_ONE, 150, 2000);

    if(getDoorG2() == OPEN){
      SetLedParam(PLedDoorG2, FLASH_TWO_INV, 150, 2000);
    }
    else
    SetLedParam(PLedDoorG2, FLASH_TWO, 150, 2000);

  }
  
  if(getDayState() == DAY){   // if it's Day

    Serial.println("It's Day \n");

    if(getDoorG1() == OPEN){
      SetLedParam(PLedDoorG1, FLASH_ONE_INV, 100, 1000);
    }
    else
    SetLedParam(PLedDoorG1, FLASH_ONE, 100, 1000);

    if(getDoorG2() == OPEN){
      SetLedParam(PLedDoorG2, FLASH_TWO_INV, 100, 1000);
    }
    else
    SetLedParam(PLedDoorG2, FLASH_TWO, 100, 1000);
  }
}

void setRelaisOn(){
  digitalWrite(IO_Relay, HIGH);
  RelaisOn=true;
  RelaisOnTime=millis();
}

void setRelaisOff(){
  digitalWrite(IO_Relay, LOW);
  RelaisOn=false;
}

bool getDoorG1(){
  return digitalRead(IO_DoorG1);
}

bool getDoorG2(){
  return digitalRead(IO_DoorG2);
}

bool getDayState(){
  return digitalRead(IO_LDR);
}