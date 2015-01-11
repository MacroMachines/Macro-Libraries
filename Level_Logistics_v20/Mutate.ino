//__________________________________________________________________________________________//



void mutate()
{
  mutateCV = analogRead(0);
  for(byte i = 0; i <= 7; i++)
  {      
    mutateTemp[i] = map(mutateCV, 0, 1023, voltages[i], mutateDestination[i]);
    //morphTemp[i] = map(morphAmount, 0, 511, mutateTemp[i], voltages[i]);
  } 
}



void setMutateDestination()
{
  //mutateGate[0]=bitRead(readInput(1),0);
  
  if(mutateGate[0] == HIGH)          // if mutate gate is high make new random destinations
  {
    for(byte i = 0; i <= 7; i++)
    {
      mutateDestination[i] = random(256);
    }
  }
  else if(mutateGate[0] == LOW)         // if mutate gate goes low make current values into voltages
  {
    for(byte i = 0; i <= 7; i++)
    {
      voltages[i] = mutateTemp[i];
    }
  }
}



