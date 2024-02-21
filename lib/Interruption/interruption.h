#pragma once

#include <Arduino.h>

enum ISR_Events {DAYSTATECHANGED, DOORG1DEBOUNCE, DOORG2DEBOUNCE, DOORG1MOVED, DOORG2MOVED, SOLVED,};

void IRAM_ATTR ISR_DoorG1Moved();
void IRAM_ATTR ISR_DoorG2Moved();
void IRAM_ATTR ISR_DayStateChanged();