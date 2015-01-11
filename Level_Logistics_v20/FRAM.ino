///////////////////////////////////////   FRAM    ////////////////////////////////////////

void save(byte location)  // NEED TO FIGURE OUT HOW TO BLOCK OUT THE AREA OF RAM FOR THE WHOLE GROUP OF PARAMETERS
{
  // save all of the things in fram
  for(byte i = 0; i < 8; i++)
  {
    fram_write(location+i, voltages[i]);  // save the voltages
  }
}


void recall(byte location)  // NEED TO FIGURE OUT HOW TO BLOCK OUT THE AREA OF RAM FOR THE WHOLE GROUP OF PARAMETERS
{
  // load all the things from fram
  for(byte i = 0; i < 8; i++)
  {
    voltages[i] = fram_read(location+i);  // load the voltages
  }
}


void fram_write(word address, byte data)
{
  Wire.beginTransmission(B1010000);
  Wire.write(highByte(address));
  Wire.write(lowByte(address));
  Wire.write(data);
  Wire.endTransmission();
}


byte fram_read(word address)
{
  byte x;
  Wire.beginTransmission(B1010000);
  Wire.write(highByte(address));
  Wire.write(lowByte(address));
  Wire.endTransmission();
  Wire.requestFrom(B1010000,1);
  x=Wire.read();
}

