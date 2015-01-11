///////////////////////////////////////   MIDI    ////////////////////////////////////////
//______________________________________________________________________________________//



void handleProgramChange(byte channel, byte number)
{  
    if(zoom==64||zoom==0)
    {
      recall(number);
    }
    
    else if(zoom==127)
    {
      save(number);
    }
    
    else if(zoom==33)
    {
      for(byte i = 0; i >= 127; i++)
      {
      save(i);
      }
    }
}





void handleControlChange(byte channel, byte number, byte value)
{
  if(number==16)  // ZOOM SAVE
  {
      if(value==0)
      {
        zoom=0;
      }
      else if(value==64)
      {
        zoom=64;
      }
      else if(value==127)
      {
        zoom=127;
      }
  }
  
   //____________________
  // Should we change this?
  if(number==17)
  {
      morphAmount=value;
  }
 
 
  if(number==18)
  {
      byte lowQuant=value;
  }
  if(number==19)
  {
      byte highQuant=value;
  }
 
 /*
  //____________________
    // !!! needs change
  if(number==51)
  {
      if(value>0)
        mutate(value);
*/
}









