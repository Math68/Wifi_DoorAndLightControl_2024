#pragma once

#include <Arduino.h>

enum Actions {DAYSTATECHANGED, DOORG1STATECHANGED, DOORG2STATECHANGED, MYTIMERHASOVERFLOW, SOLVED, DEBOUNCEG1, DEBOUNCEG2};


void IRAM_ATTR onmyTimerOverflow();

void IRAM_ATTR onDoorG1StateChanged();
void IRAM_ATTR onDoorG2StateChanged();

void IRAM_ATTR onDayStateChanged();