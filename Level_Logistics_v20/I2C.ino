///////////////////////////////////////   I2C    ////////////////////////////////////////



void sequenceOut(int value)
{
  Wire.beginTransmission(B1100001);
  Wire.write(value>>8);
  Wire.write(value&255);
  Wire.endTransmission();
}



void setSlew(byte amount)
{
  Wire.beginTransmission(B0101000);
  Wire.write(0);
  Wire.write(amount);
  Wire.endTransmission();
}



void mutateAttenuate(byte level)
{
  Wire.beginTransmission(B0101000);
  Wire.write(B00010001);
  Wire.write(level);
  Wire.endTransmission();
}



byte intFlag(byte channel)
{
  Wire.beginTransmission(0x20);
  
  if(channel==0)
    Wire.write(0x0E);
  else
    Wire.write(0x0F);
    
  Wire.endTransmission();
  Wire.requestFrom(0x20,1);
  return Wire.read();
}



byte readInput(byte channel)
{
  Wire.beginTransmission(0x20);
  
  if(channel==0)
    Wire.write(0x10);
  else
    Wire.write(0x11);
    
  Wire.endTransmission();
  Wire.requestFrom(0x20,1);
  return Wire.read();
}



void dac(byte channel, int value)
{
  Wire.beginTransmission(B1100000);
  Wire.write(B01000001|((channel%4)<<1));
  Wire.write(B00000000|(value>>8));
  Wire.write(value&255);
  Wire.endTransmission();
}



void IORegister(byte writeRegister, byte value)
{
  Wire.beginTransmission(0x20);
  Wire.write(writeRegister);
  Wire.write(value);
  Wire.endTransmission();
}
