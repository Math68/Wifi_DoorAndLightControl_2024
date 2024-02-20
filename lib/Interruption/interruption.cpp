#include "interruption.h"

// ********** INTERRUPTION FUNCTIONS ***************

Actions Action;

void IRAM_ATTR onmyTimerOverflow(){
    Action=MYTIMERHASOVERFLOW;
    //Serial.println("Interruption, Timer has overflow");
}

void IRAM_ATTR onDoorG1StateChanged(){
        Action=DEBOUNCEG1;
        Serial.println("Interruption, Door 1 State Changed: ");
}

void IRAM_ATTR onDoorG2StateChanged(){
    Action=DOORG2STATECHANGED;
    //Serial.println("Interruption, Door 2 State changed");
}

void IRAM_ATTR onDayStateChanged(){
    Action=DAYSTATECHANGED;
    //Serial.println("Interruption, Day State Changed");
}