



/*
void TimeManager(){
  if(Timer0HasOverflow == true){  // All 50ms
    Timer0HasOverflow = false;

    // ******** AliveLedTimeStep **********
  //  if(AliveLedTimeStep>0)
    //  AliveLedTimeStep = AliveLedTimeStep - 1;

    // ******** RelayTimeStep *************
    //if(RelayDelay>0)
//      RelayDelay = RelayDelay - 1;

    // ******** LDR Reading Timing *******
//    if(LdrReadingInterval>0)
  //    LdrReadingInterval = LdrReadingInterval - 1;
  }
}


void SetAliveLed(){
  if(AliveLedTimeStep == 0){
    
    if(AliveLedStatus == LOW){
      AliveLedTimeStep = 2;        // high state 2x50ms=100ms
      AliveLedStatus = HIGH;
      digitalWrite(IO_AliveLed, LOW);
    }
    else{
      if(DayState == NIGHT)      // low state at night = 1900ms
        AliveLedTimeStep = 38;
      else
        AliveLedTimeStep =18;   // low state at day = 900ms
            
      AliveLedStatus = LOW;
      digitalWrite(IO_AliveLed, HIGH);
    }
  }  
}
*/

/*
void SetDayState(){    
  if(LdrReadingInterval==0){
    LdrReadingInterval=40;  // => 2s  for 5mn = 300s => 6000, 15mn = 900s => 18000
    LdrValue = analogRead(IO_LDR);

    if(DayState == DAY){
      if(LdrValue<DayStateThresholdLow)
        DayState=NIGHT;
    }
    else{
        if(LdrValue>DayStateThresholdHigh)
          DayState=DAY;
    }
  }
}
*/

/*
void setRelayON(){
  if(DayState == NIGHT){
    digitalWrite(IO_Relay, LOW);  // Relay Actvation for 1s
    RelayDelay = 20;
  }
}

void notifyClientsToRefreshPictures(String Data);

void CheckDoor(){
  if(Door1HasMoved == false){             // if Door did not move
    if(digitalRead(IO_Door1) == HIGH){    // if Door Open
      Door1HasMoved = true;               // Set Variable
      Door1Delay=5;                       // Set Delay 250ms
    }
  }
  else{                                    // if door has moved
    if(EventHandled == true){              // if door opening handled
      if(digitalRead(IO_Door1)==0){
        EventHandled = false;              // EventHandled reset after door is closed
        // Door1 has been closed, Picture has to be refreshed...
        notifyClientsToRefreshPictures(doorsState());
        Serial.print("Door Closed \n");
      }    
    }
    else{                                   // After 
      if(Door1Delay == 0){                  // after debounce time 
        if(digitalRead(IO_Door1)==1){       // If Door still open
          setRelayON();
          EventHandled = true;              // Set Event Handler
          // Door1 has been opened, Picture has to be refreshed...
          notifyClientsToRefreshPictures(doorsState());            
          Serial.print("Door Opened \n");
        }
      }
    } 
  }
}
  

void setRelayOff(){
  if(RelayDelay == 0)
    digitalWrite(IO_Relay, HIGH);
}
*/