
int blinkSpeed=220;     // clock division blink speed


void LEDoutput()   // Sets shift register to view current setting on LEDs 
{
    byte seqOut;
    byte seq1LED=(setting[activeSetting][6])%4;
    byte seq2LED=(setting[activeSetting][7])%4;
    byte channel1LED=(setting[activeSetting][0]+clockPosition[0])%4;
    byte channel2LED=(setting[activeSetting][2]+clockPosition[1])%4;
    stepOut=positionLED[0][channel1LED]+positionLED[1][channel2LED];
    seqOut=positionLED[0][seq1LED]+positionLED[1][seq2LED];
    
    if (zoom==0||(zoomTemp==0&&zoom==64)) {
      byte patternLED;
      byte tempLED;
      byte highLED=stepPattern[setting[activeSetting][3]]<<4;
      byte lowLED=stepPattern[setting[activeSetting][1]];
      patternLED=highLED+lowLED;
      tempLED=patternLED;
     
      if(millis()<clockCalc[0]+60)
        bitClear(tempLED,((setting[activeSetting][0]+clockPosition[0])%4));
      if(millis()<clockCalc[1]+60)
        bitClear(tempLED,(setting[activeSetting][2]+clockPosition[1])%4+4);
     
      unsigned long maxClock;
      maxClock=max(clockCalc[0],clockCalc[1]);
      
      if(millis()<maxClock+100){
        bitClear(PORTB,0);
        shiftOut(dataPin, clockPin, MSBFIRST, tempLED);   
        bitSet(PORTB,0);
        }
      else {
        bitClear(PORTB,0);
        shiftOut(dataPin, clockPin, MSBFIRST, patternLED);   
        bitSet(PORTB,0);
    }
  }
  else if(zoom==127||(zoomTemp==127&&zoom==64)) {
    if (setting[activeSetting][4]==4) {chan1=0;}
    else {chan1=positionLED[0][setting[activeSetting][4]]; }
    if (setting[activeSetting][5]==4) {chan2=0;}
    else {chan2=positionLED[1][setting[activeSetting][5]]; }
   
    if((millis()-zoomTime)%blinkSpeed>blinkSpeed/2)  {
      bitClear(PORTB,0);
      shiftOut(dataPin, clockPin, LSBFIRST, (chan1+chan2));
      bitSet(PORTB,0); }
    else {
      bitClear(PORTB,0);
      shiftOut(dataPin, clockPin, LSBFIRST, 0);
      bitSet(PORTB,0); }
  }
  else if (zoomTemp==64&&zoom==64)
  {
    bitClear(PORTB,0);
    shiftOut(dataPin, clockPin, LSBFIRST, (stepOut));   
    bitSet(PORTB,0);
  }
  else if(zoomTemp==126&&zoom==64) // sequencing mode
  {
     if(millis()%(blinkSpeed/4)>(blinkSpeed/8))
     {
     chan1=positionLED[0][setting[activeSetting][6]];
     chan2=positionLED[1][setting[activeSetting][7]];
     }
     else
     {
     chan1=0;
     chan2=0;
     }
     byte tempLED;
     tempLED=(chan1+chan2);
     
     if(millis()<clockCalc[0]+60) {
       if(setting[activeSetting][6]==(setting[activeSetting][0]+clockPosition[0])%4){
         bitClear(tempLED,reverse[((setting[activeSetting][0]+clockPosition[0])%4)]);}
       else{
         bitSet(tempLED,reverse[((setting[activeSetting][0]+clockPosition[0])%4)]);}
     }
     if(millis()<clockCalc[1]+60){
       if(setting[activeSetting][7]==(setting[activeSetting][2]+clockPosition[1])%4+4){
         bitClear(tempLED,reverse[((setting[activeSetting][2]+clockPosition[1])%4+4)]);}
       else{
         bitSet(tempLED,reverse[((setting[activeSetting][2]+clockPosition[1])%4+4)]);}
    }
    bitClear(PORTB,0);
    shiftOut(dataPin, clockPin, LSBFIRST, (tempLED));   
    bitSet(PORTB,0);
  }
}
