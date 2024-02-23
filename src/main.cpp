// 23/02/2024 8H15
// Last Update ?? Version definitive

#include <Arduino.h>
#include <Arduino_JSON.h>
#include "ledController.h"
#include "interruption.h"
#include "websocket.h"
#include "password.h"
#include "debug.h"

#include <WiFi.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>

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
bool getDoorG1State();
bool getDoorG2State();
bool getDayState();

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
DoorAndLightWebsocket websocket(&server, &ws);

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

  // Interrupt Service Routine Initialisation
  ISR_Solved;

  // ***********************************************
  // ********** WEBSOCKET **************************
  // ***********************************************
  
  // ********** Spiffs **********
  if(!SPIFFS.begin()){
    debugln("Erreur SPIFFS...");
    return;
  }

  File root = SPIFFS.open("/");
  File file = root.openNextFile();

  while(file){
    debug("File: ");
    debugln(file.name());
    file.close();
    file = root.openNextFile();
  }

  // ********** Wifi **********
  WiFi.begin(ssid, password);
  debug("Tentative de connexion...");

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    debug(".");
  }

  debugln("\n");
  debugln("Connexion etablie...");
  debug("Adresse IP: ");
  debugln(WiFi.localIP());

  websocket.initWebSocket();

  // ********** Server **********
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/w3.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/",SPIFFS, "/");
  server.begin();
  debugln("Serveur actif!");
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
        if(getDoorG1State()==HIGH)
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
  ws.cleanupClients();
}

// ***************** FUNCTIONS ******************

void setLedState(){
  if(getDayState() == NIGHT){

    Serial.println("It's Night \n");

    if(getDoorG1State() == OPEN){
      SetLedParam(PLedDoorG1, FLASH_ONE_INV, 150, 2000);
    }
    else
    SetLedParam(PLedDoorG1, FLASH_ONE, 150, 2000);

    if(getDoorG2State() == OPEN){
      SetLedParam(PLedDoorG2, FLASH_TWO_INV, 150, 2000);
    }
    else
    SetLedParam(PLedDoorG2, FLASH_TWO, 150, 2000);

  }
  
  if(getDayState() == DAY){   // if it's Day

    Serial.println("It's Day \n");

    if(getDoorG1State() == OPEN){
      SetLedParam(PLedDoorG1, FLASH_ONE_INV, 100, 1000);
    }
    else
    SetLedParam(PLedDoorG1, FLASH_ONE, 100, 1000);

    if(getDoorG2State() == OPEN){
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

bool getDoorG1State(){
  return digitalRead(IO_DoorG1);
}

bool getDoorG2State(){
  return digitalRead(IO_DoorG2);
}

bool getDayState(){
  return digitalRead(IO_LDR);
}