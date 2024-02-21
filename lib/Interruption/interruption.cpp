#include "interruption.h"

// ********** INTERRUPTION FUNCTIONS ***************

ISR_Events ISR_Event;

void IRAM_ATTR ISR_DoorG1Moved(){
    ISR_Event=DOORG1DEBOUNCE;
    //Serial.println("ISR, Door 1 Moved: ");
}

void IRAM_ATTR ISR_DoorG2Moved(){
    ISR_Event=DOORG2DEBOUNCE;
    //Serial.println("Interruption, Door 2 State changed");
}

void IRAM_ATTR ISR_DayStateChanged(){
    ISR_Event=DAYSTATECHANGED;
    //Serial.println("Interruption, Day State Changed");
}