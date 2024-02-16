#pragma once

#include <Arduino.h>
#include <Arduino_JSON.h>
//#include <WiFi.h>
//#include <AsyncTCP.h>
//#include <ESPAsyncWebServer.h>

#include "ledController.h"

#define DAY 1
#define NIGHT 0

#define CLOSED 0
#define OPEN 1

enum Actions {DAYSTATECHANGED, DOORG1OPENED, DOORG1CLOSED, DOOG2OPENED, DOORG2CLOSED, SOLVED};

// ********** SET PIN NUMBERS **********
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

// ********** SET VARIABLES ***********
extern bool dayCycleStatus;
extern bool DoorG1_Status;
extern bool DoorG2_Status;

extern LedParam *PLedDoorG1;
extern LedParam *PLedDoorG2;

// ********** FUNCTIONS ***************

bool getDayState();
void getDoorState();
void setLedState();
//void setLedState(struct LedParam *_Pled);


//
// ********** END *********************
//


/*


int LdrReadingInterval = 20;   // Time interval between two Day Status check
int LdrValue = -1;

int DayState = NIGHT;
int DayStateThresholdHigh = 2100;
int DayStateThresholdLow = 1900;

int RelayDelay=0;

int Door1HasMoved=-1;
int Door1Delay=0;
int EventHandled = false;

String DoorMathState="Closed";
String DoorCaroState="Closed";

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