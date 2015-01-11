

void handleEncoder()
{
  switch (buttons) {
    case buttonEncoder:
      selectedSetting = encoder(selectedSetting, 1, 0, 7);
      break;
    case buttonMutate:
      mutateAmount = encoder(mutateAmount, 16, 0, 127);
      mutateAttenuate(abs(mutateAmount-128));    // invert
      break;
    case buttonRotate:
      rotate[selectedSetting] = encoder(rotate[selectedSetting], 1, 0, 7);
      break;
    case buttonRepeat:
      repeat[selectedSetting] = encoder(repeat[selectedSetting], 1, 0, 7);
      break;
    case buttonLength:
      length[selectedSetting] = encoder(length[selectedSetting], 1, 0, 7);
      break;
    case buttonGate:
      gate[selectedSetting] = encoder(gate[selectedSetting], 1, 0, 7);
      break;
    case buttonGlide:
      glide[selectedSetting] = encoder(glide[selectedSetting], 1, 0, 7);
      break;
    default:
      voltages[selectedSetting] = encoder(voltages[selectedSetting], 16, 0, 255);
      break;
  }
}




int encoder(int value, int increment, int minimum, int maximum)
{
  
  if(buttons != buttonEncoder && buttons != buttonMutate)
  {
    bargraphCountDown = millis();
    
    if(maximum == 7)
      currentAdjustmentValue = abs(value-7);
    else if(maximum == 255)
      currentAdjustmentValue = abs(((value+1)/32)-7);
  }
  
  if ((newPosition > oldPosition+3)||(newPosition < oldPosition-3)) 
  {
    if(newPosition > oldPosition+3)
      value=constrain((value+increment),minimum,maximum);
    else if(newPosition < oldPosition-3)
      value=constrain((value-increment),minimum,maximum);
    
    oldPosition = newPosition;
  }     
  return value;
}




