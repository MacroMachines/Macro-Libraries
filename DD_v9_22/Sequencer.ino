
void reset1()
{
  clockPosition[0]=0;                      // resets the position of channel 1 to 0
  resetTime[0]=micros();                            // finds timing of rising edge
}


void reset2()
{
  clockPosition[1]=0;                       // resets the position of channel 2 to 0
  resetTime[1]=micros();                             // finds timing of rising edge
}



void clock()                                        // advances the current position in the switch when a clock pulse happens
{
  for(int c=0; c<2; c++){
    if(bitRead(PIND,c+6) && zoom != 127){
      while(clkCount[c] < 1){
        clockCalc[c]=millis();
        if(micros()>resetTime[c]+1000){              // only allow clock to advance if reset was longer then 1000 micro seconds ago
          if(divideCount[c]%(divideMap[setting[activeSetting][c+4]])==0){
            if(setting[activeSetting][c+4]!=4){
              do {
                 switch(setting[activeSetting][c+6])
                 {
                   case 0: //fwd
                    up(c);
                    break;
                   case 1: //rev
                    down(c);
                    break;
                   case 2:
                   if(setting[activeSetting][c*2+1]<5)
                   {
                     if(setting[activeSetting][c*2+1]==1)
                     {
                     if((setting[activeSetting][c*2]+clockPosition[c])%4==0)
                     {
                     pend[c]=0;
                     }
                     else if((setting[activeSetting][c*2]+clockPosition[c])%4==2)
                     {
                     pend[c]=1;
                     }
                     }
                     else if(setting[activeSetting][c*2+1]==4)
                     {
                     if((setting[activeSetting][c*2]+clockPosition[c])%4==1)
                     pend[c]=0;
                     else if((setting[activeSetting][c*2]+clockPosition[c])%4==3)
                     pend[c]=1;
                     }
                     else
                     {
                   if(((setting[activeSetting][c*2]+clockPosition[c])%4==bounds[0][setting[activeSetting][c*2+1]]))
                    pend[c]=0;
                   if(((setting[activeSetting][c*2]+clockPosition[c])%4==bounds[1][setting[activeSetting][c*2+1]]))
                    pend[c]=1;
                     }
                   }
                   else
                   pend[c]=0;
                   switch(pend[c])
                   {
                   case 0:
                   up(c);
                   break;
                   case 1:
                   down(c);
                   break;
                   }
                   break;
                   case 3: //random
                    clockPosition[c]=clockPosition[c]+random(4);
                    break;
                 }
                 }
              while(bitRead(stepPattern[setting[activeSetting][(c*2)+1]], (setting[activeSetting][c*2]+clockPosition[c])%4) == 0);}} // resctricting to step pattern
          divideCount[c]++;
          clkCount[c]++;
        }
      }
    }
    else {clkCount[c] = 0;} 
    }
}

void up(byte c)
{
            clockPosition[c]++; 
}

void down(byte c)
{
            clockPosition[c]--;
}

