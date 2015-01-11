
//__________________________________________________________________________________________//



/*
void checkButtons()
{
  byte tempByte=readInput(0);
  buttonEncoder=bitRead(tempByte,0);
  buttonMutate=bitRead(tempByte,1);
  buttonLength=bitRead(tempByte,2);
  buttonGlide=bitRead(tempByte,3);
  buttonGate=bitRead(tempByte,4);
  buttonRepeat=bitRead(tempByte,5);
  buttonRotate=bitRead(tempByte,6);
}
*/




//////////////////////////////NOT SURE???
/*
void LEDout()
{
  int blinkDuration=200;
  
  if(LEDmode==1)                           // blinking LED = Mode 1 (when encoder button is pressed)
  {
    for(int i=0;i<13;i++)
    {
      if(i==selectedSetting)
      {
        byte tempLED=voltages[activeSetting][i]/16;
        if(millis()%blinkDuration<blinkDuration/2)
        {
          if(i<8)
          {
            ShiftPWM.SetOne(i,tempLED);
          }
        }
        
        else
          ShiftPWM.SetOne(i,(tempLED+128)%255);
      }
      else
        ShiftPWM.SetOne(i,voltages[activeSetting][i]/16);
     }
   }
   
   
   else if(LEDmode==2)                      // bar display  = Mode 2 (encoder is turned)
   {
     if(selectedSetting<8)
     {
       for(int r=0;r<8;r++)
       {
          byte revArray[8]={7,6,5,4,3,2,1,0};
          
          if(voltages[selectedSetting][0]>512*r&&voltages[selectedSetting][0]<(512*(r+1)))    /////////??? NEED TO CHANGE 512???????
             ShiftPWM.SetOne(revArray[r],map((voltages[selectedSetting][0]-(512*r)),0,(511*(r+1))-1,0,255));
          else if(voltages[selectedSetting][0]>=512*(r+1))
             ShiftPWM.SetOne(revArray[r],255);
          else
             ShiftPWM.SetOne(revArray[r],0);
        }
      }
    }
}
*/


/*
//////////////////////////////NOT SURE???
void barDisplay()
{
  for(int r=0;r<8;r++)
  {
    if(voltages[activeSetting][select]>512*r&&voltages[activeSetting][select]<(512*(r+1)))
      ShiftPWM.SetOne(map(r,0,7,7,0),map((voltages[activeSetting][select]-(512*r)),0,(511*(r+1))-1,0,flashBrightness));
    else if(voltages[activeSetting][select]>=512*(r+1))
      ShiftPWM.SetOne(map(r,0,7,7,0),flashBrightness);
    else
      ShiftPWM.SetOne(map(r,0,7,7,0),0);
  }
}
*/



