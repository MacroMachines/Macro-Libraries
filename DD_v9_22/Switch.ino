void switchLogic()                                   //Writes logic to Analog Devices ADG5204 switch chips
{
  byte channel1logic=(setting[activeSetting][0]+clockPosition[0])%4;
  byte channel2logic=(setting[activeSetting][2]+clockPosition[1])%4;

  bitWrite(PORTD,4,(channel1logic&B01));   //current setting channel 1 position (bit 1)
  bitWrite(PORTD,5,(channel1logic&B10));   //current setting channel 1 position (bit 2)
  bitWrite(PORTD,2,(channel2logic&B01));   //current setting channel 2 position (bit 1)
  bitWrite(PORTD,3,(channel2logic&B10));   //current setting channel 2 position (bit 2)
}

