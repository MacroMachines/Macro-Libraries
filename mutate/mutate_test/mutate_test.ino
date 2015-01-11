#include <PinChangeInt.h>
#include <Wire.h>



int voltages[8][2] = {(random(256),random(256),random(256),random(256),random(256),random(256),random(256),random(256)),
                      (random(256),random(256),random(256),random(256),random(256),random(256),random(256),random(256))};        // voltages for dacs and sequencer

int mutateDestination[8] = {random(256),random(256),random(256),random(256),random(256),random(256),random(256),random(256)};   // 1 - 8 mutate 1-8 values, above that mutate all
int mutateAmount;          // Depth of Mutation 
int mutate[8];
  
int morphAmount;
int morph[8];


  
void setup()
{
  Wire.begin();
  pinMode(A2,INPUT);
  PCintPort::attachInterrupt(A2,&setMutateDestination,CHANGE);
}



void loop()
{
  for(byte i = 0; i <= 7; i++)
  {
    mutateAmount = analogRead(1);
    morphAmount = analogRead(0);
      
    mutate[i] = map(mutateAmount, 0, 1023, voltages[i][0], mutateDestination[i]);
    morph[i] = map(morphAmount, 0, 511, mutate[i], voltages[i][1]);
    
    if(i < 4)
    {
      dac(1, i, morph[i]<<4);
    }
    else
    {
      dac(2, i-4, morph[i]<<4);
    }
    
  } 
}



void setMutateDestination()
{
  if(digitalRead(A2))          // if mutate gate is high make new random destinations
  {
    for(byte i = 0; i <= 7; i++)
    {
      mutateDestination[i] = random(256);
    }
  }
  else                        // if mutate gate goes low make current values into voltages
  {
    for(byte i = 0; i <= 7; i++)
    {
      voltages[i][0] = mutate[i];
    }
  }
}


void dac(byte chip, byte channel, int value)
{
  Wire.beginTransmission(B1100000+chip);
  Wire.write(B01000000|(channel<<1));
  Wire.write(B00000000|(value>>8));
  Wire.write(value&255);
  Wire.endTransmission();
}

