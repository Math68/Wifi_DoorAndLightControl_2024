#include "interruption.h"

// ********** INTERRUPTION FUNCTIONS ***************

void IRAM_ATTR onmyTimer0Overflow(){
    //myTimer0HasOverflow = true;
}

void IRAM_ATTR onmyTimer1Overflow(){
}

void IRAM_ATTR onDoorG1Opened(){
    Action=DOORG1OPENED;
}

void IRAM_ATTR onDoorG1Closed(){
    Action=DOORG1CLOSED;
}

void IRAM_ATTR onDoorG2Opened(){
    Action=DOOG2OPENED;
}

void IRAM_ATTR onDoorG2Closed(){
    Action=DOORG2CLOSED;
}

void IRAM_ATTR onDayStateChanged(){
    Action=DAYSTATECHANGED;
    Serial.println("Interruption, Day State Changed");
}