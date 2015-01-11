

void buttons(){
  buttonReading[0][0]=buttonReading[0][1];  // set history of button reading into first part of array
  buttonReading[0][1]=bitRead(PINC,0);      // set current value of button 1
  buttonReading[1][0]=buttonReading[1][1];  // set history of button reading into first part of array
  buttonReading[1][1]=bitRead(PINC,1);      // set current value of button 2
  
  for(int i;i<2;i++){
    if(buttonReading[i][0]!=buttonReading[i][1]&&buttonReading[i][1]==0){ // button rising edge
      downTime[i]=millis();
      divideCount[i]=0;  }                                               // reset divide counter when button is pressed
  if(buttonReading[0][1]==0&&buttonReading[1][1]==0){
  
    while(buttonHold<1)  {
      holdTime=millis();
      switch(zoomTemp){                                                  // Step through modes by pressing both buttons
        case 0:
          zoomTemp=127;
          break;
        case 64:
          zoomTemp=0;
          break;
        case 127:
          zoomTemp=126;
          break;
        case 126:
          zoomTemp=64;
          break;
          }
      buttonHold++;
    }
  }
  else if(buttonReading[0][1]==1&&buttonReading[1][1]==1)
    buttonHold=0;
  
  if(millis()>holdTime+30) if(buttonReading[i][1]==0) if(buttonHold==0) if(millis()>downTime[i]+30) { //amount of time needed to hold button to advance step
      while(buttonCount[i]<1) {
        if(zoom==0||(zoomTemp==0&&zoom==64)) {                               // if bank button is down (zoom -) we are in length mode
          setting[activeSetting][i*2+1]=(setting[activeSetting][i*2+1]+1)%11;}
        else if(zoom==127||(zoomTemp==127&&zoom==64)){
          setting[activeSetting][i+4]=(setting[activeSetting][i+4]+1)%5; } //increments and sets clock divide between 0 and 4 (off, divide by 1,2,4, or 8)
        else if(zoomTemp==126&&zoom==64){
          setting[activeSetting][i+6]=(setting[activeSetting][i+6]+1)%4; }
        else {                                                 // if not we are setting the current switch position
          setting[activeSetting][i*2]=(setting[activeSetting][i*2]+1+clockPosition[i])%4;
          clockPosition[i]=0; }
          buttonCount[i]++;  }
  }
  else { buttonCount[i]=0;}
  }
}
