#pragma once

#include <Arduino.h>

void IRAM_ATTR onmyTimer0Overflow();
void IRAM_ATTR onmyTimer1();
void IRAM_ATTR DG1_SetRelay();
void IRAM_ATTR DG2_SetRelay();