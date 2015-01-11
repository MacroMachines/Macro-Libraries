void handleProgramChange(byte channel, byte number){
  //zoomTemp=64;
    if(zoom == 127){            // if "save one" (zoom in) is high     
      if(activeSetting==number){// if storing into the same preset that is currently selected
        for(int e=0; e<8; e++){
          if(e==0||e==2){
            if(e==0){
              EEPROM.write((number*8)+e,(setting[number][0]+clockPosition[0])%4);
              setting[activeSetting][0]=(setting[number][0]+clockPosition[0])%4;}
            if(e==2){ 
              EEPROM.write((number*8)+e,(setting[number][2]+clockPosition[1])%4);
              setting[activeSetting][2]=(setting[number][2]+clockPosition[1])%4;}}
          else{
            EEPROM.write((number*8)+e,setting[number][e]);}}}
      else{                                   //if saving into a different location than currently selected
        for(int e=0;e<8;e++){
          if(e==0||e==2){
            if(e==0){
              EEPROM.write((number*8)+e,(setting[activeSetting][0]+clockPosition[0])%4);
              setting[number][0]=(setting[activeSetting][0]+clockPosition[0])%4;}
            if(e==2) {
              EEPROM.write((number*8)+e,(setting[activeSetting][2]+clockPosition[1])%4);
              setting[number][2]=(setting[activeSetting][2]+clockPosition[1])%4;}
                       }
            else{
              EEPROM.write((number*8)+e,setting[activeSetting][e]);
              setting[number][e]=setting[activeSetting][e];}}}}
    else{activeSetting=number;}               // if were not saving one then just set the active slot
    for(int p=0; p<2; p++){
      pend[p]=0;
      clockPosition[p]=0;}
}



void handleControlChange(byte channel, byte number, byte value)
{
    if(number==16){ // zoom cc
      if(zoomTime >= (millis()-20)){
        if((value==64 && zoom==0) || (value==64 && zoom==127)){
          zoomTemp = 126;}}
      else{zoomTemp=value;}
      
      zoom=value;
      zoomTime=millis();
    
      if(value==33){
        saveAll();}
    }    
      
    if(number==18){ // mutate cc
      if(value){
        mutate(value);}}
}




void saveAll(){  
  for(int i;i<64;i++){
  for(int x=0;x<8;x++){
    if(i==activeSetting&&(x==0||x==2)){
      if(x==0) {
          EEPROM.write((i*8),(setting[i][0]+clockPosition[0])%4);
          setting[i][0]=(setting[i][0]+clockPosition[0])%4;
          clockPosition[0]=0; }
        if(x==2) {
          EEPROM.write((i*8)+2,(setting[i][2]+clockPosition[1])%4);
          setting[i][2]=(setting[i][2]+clockPosition[1])%4;
          clockPosition[1]=0;  }  }
      else{
        EEPROM.write((i*8)+x,setting[i][x]);
      }
    }
  }
}





void mutate(byte mutateAmount){
  if(mutateAmount > random(127)){
    setting[activeSetting][0]=(setting[activeSetting][0]+random((mutateAmount/32)+1))%4;   // position 1
    setting[activeSetting][1]=(setting[activeSetting][1]+random(mutateAmount/12))%11;      // pattern 1
    setting[activeSetting][4]=(setting[activeSetting][4]+random(mutateAmount/26))%5;       // division 1
    setting[activeSetting][6]=(setting[activeSetting][6]+random(mutateAmount/43))%4;}      // sequence mode 1
  
  if(mutateAmount > random(127)){      
    setting[activeSetting][2]=(setting[activeSetting][2]+random((mutateAmount/32)+1))%4;   // position 2
    setting[activeSetting][3]=(setting[activeSetting][3]+random(mutateAmount/12))%11;      // pattern 2
    setting[activeSetting][5]=(setting[activeSetting][5]+random(mutateAmount/26))%5;       // division 2
    setting[activeSetting][7]=(setting[activeSetting][7]+random(mutateAmount/43))%4;}    // sequence mode 2 
}
