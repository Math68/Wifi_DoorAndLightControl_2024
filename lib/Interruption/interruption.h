#pragma once

#include <Arduino.h>
#include "wifi_2024.h"

//volatile int myTimer0HasOverflow = false;
//volatile int myTimer1HasOverflow = false;

extern Actions Action;

void IRAM_ATTR onmyTimer0Overflow();
void IRAM_ATTR onmyTimer1Overflow();

void IRAM_ATTR onDoorG1Opened();
void IRAM_ATTR onDoorG1Closed();
void IRAM_ATTR onDoorG2Opened();
void IRAM_ATTR onDoorG2Closed();

void IRAM_ATTR onDayStateChanged();