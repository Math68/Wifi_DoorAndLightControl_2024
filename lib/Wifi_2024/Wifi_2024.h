#pragma once

#include <Arduino.h>
#include <Arduino_JSON.h>
//#include <WiFi.h>
//#include <AsyncTCP.h>
//#include <ESPAsyncWebServer.h>

#define DAY 1
#define NIGHT 0
//#define LOW 0

// ********** SET PIN NUMBERS **********
// 34 -> 39 only inputs
// Inputs
const int IO_Door1 = 26;
const int IO_Door2 = 14;
const int IO_LDR = 15;   // ADC1_CH
const int IO_Home = 12;

// Outputs
const int IO_Relay = 25;
const int IO_GD1_Status = 19;
const int IO_GD2_Status = 17;

// ********** SET VARIABLES ***********




//
// ********** END *********************
//


/*
int AliveLedStatus = LOW;
int AliveLedTimeStep = 0;

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