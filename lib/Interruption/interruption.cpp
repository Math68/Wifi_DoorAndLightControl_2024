#include "interruption.h"
#include "wifi_2024.h"

volatile int myTimer0HasOverflow = false;

// ********** INTERRUPTION FUNCTIONS ***************

void IRAM_ATTR onmyTimer0Overflow(){
    myTimer0HasOverflow = true;
}

void IRAM_ATTR onmyTimer1(){
  //digitalWrite(IO_Relais, HIGH);
}

void IRAM_ATTR DG1_SetRelay(){

}

void IRAM_ATTR DG2_SetRelay(){

}
