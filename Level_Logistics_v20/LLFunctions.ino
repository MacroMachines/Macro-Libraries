///////////////////////////////////////   FUNCTIONS    ////////////////////////////////////////
//__________________________________________________________________________________________//





void LEDout()
{
  for(byte i = 0; i <= 7; i++) 
  {  
    if(buttons == buttonEncoder)                // _____SELECTCURRENT MODE________________
    {
      if(i == selectedSetting)
        {LEDlevel[i] = 255;}         
      else
        {LEDlevel[i] = 0;}     
    }  
    else if(buttons != buttonEncoder && (millis() - bargraphCountDown) < 500)  // _____BARGRAPH MODE_________________
    {
      if(i > currentAdjustmentValue)
        {LEDlevel[i] = 255;}
      else
        {LEDlevel[i] = 0;}
    }
    else                                         // _____DEFAULT MODE_________________
    {        
      if(i == currentStep)
        {LEDlevel[i] = 255;} // add brighter value for current step
      else
        {LEDlevel[i] = (mutateTemp[i]>>3);}
    }    
    ShiftPWM.SetOne(i,LEDlevel[i]);   // use this function to set any LED 0-7, value 0-255  
  }
}






void checkGates()
{
  byte tempByte=readInput(1);
  clockInput[0]=bitRead(tempByte,2);
  resetInput[0]=bitRead(tempByte,1);
  mutateGate[0]=bitRead(tempByte,0);
}





void sequencer()
{
  if((currentStep > 7) || (resetInput[0] == HIGH && resetInput[1] == LOW))
  {
    currentStep = 0;
  }
  else if(clockInput[0] == HIGH && clockInput[1] == LOW)
  {
    currentStep=currentStep+1;
  }
  //digitalWrite(7,gateOut);                   // gate output set by gateOut variable on digital pin 7
}




void resetSeq()
{
  if(resetInput[0])
  {
    currentStep=0;
  }
}




void voltageOutputs()
{ 
  //dac output
  dac(0,mutateTemp[0]<<4);
  dac(1,mutateTemp[1]<<4);
  dac(2,mutateTemp[2]<<4);
  dac(3,mutateTemp[3]<<4);
  bitWrite(PORTD, 5, 0);
  bitWrite(PORTD, 5, 1);
  dac(4,mutateTemp[4]<<4);
  dac(5,mutateTemp[5]<<4);
  dac(6,mutateTemp[6]<<4);
  dac(7,mutateTemp[7]<<4);
  bitWrite(PORTD, 6, 0);
  bitWrite(PORTD, 6, 1);
  //end dac output
}


