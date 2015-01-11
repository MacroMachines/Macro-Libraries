#include <EEPROM.h>
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();
#include <PinChangeInt.h>

//shift register pins
byte latchPin = 8;
byte clockPin = 12;
byte dataPin = 11;

boolean clkCount[2];                 //bit for finding rising edge of clocks
byte divideCount[2];                 //counter for clock division
byte divideMap[5]={1,2,4,8,0};
const byte blinkDuration[2]={60,60};
byte chan1;
byte chan2;
unsigned long clockCalc[2];
byte stepOut;                        //LED output for sequencing
byte newRandom;
byte lastRandom;                     //used in random sequencing mode
byte lastPosition[2];
byte pend[2];
unsigned long zoomTime = millis();

byte activeSetting;                  // the ACTIVE SETTING NUMBER!!!
byte setting [64][8];               //0,2 = current switch position. 1,3 = step pattern. 4 = chan 1 division. 5 = chan 2 division. 6,7 = sequence mode
byte zoom=64;                        //cc #16 value for save and recall
byte zoomTemp=64;
byte clockPosition[2];
boolean doOnce=0;
byte reverse[8]={7,6,5,4,3,2,1,0};

const byte positionLED[2][4]={{B10000000,B01000000,B00100000,B00010000},{B1000,B0100,B0010,B0001}};   //array of current position indication
const byte stepPattern[11]={B1111,B0111,B1011,B1101,B1110,B0011,B1001,B1100,B0110,B0101,B1010};
const byte bounds[2][11]={{0,1, 0, 0, 0, 2, 0, 0, 1, 1, 0},{3, 3, 3, 3, 2, 3, 3, 1, 2, 3, 2}};
//byte skipCount[11]={2,1,2,}
unsigned long resetTime[2];           // time of the reset time happened

boolean buttonReading[2][2];          // [button channel] / [previous,current]
unsigned long downTime[2]={0,0};
boolean buttonCount[2];
boolean buttonHold;
unsigned long holdTime;

void setup()
{
  // MIDI setup handlers
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleProgramChange(handleProgramChange);
  MIDI.begin(1);
  
  // switch logic pins
  pinMode(2,OUTPUT); // channel 2 bit 1
  pinMode(3,OUTPUT); // channel 2 bit 2
  pinMode(4,OUTPUT); // channel 1 bit 1
  pinMode(5,OUTPUT); // channel 1 bit 2
 
  //clock inputs
  pinMode(6,INPUT_PULLUP);//channel 1 clock
  pinMode(7,INPUT_PULLUP);//channel 2 clock
  
  //reset 1 interrupt
  pinMode(9,INPUT_PULLUP);
  PCintPort::attachInterrupt(9,&reset1,RISING);
  
  //reset 2 interrupt
  pinMode(10,INPUT_PULLUP);
  PCintPort::attachInterrupt(10,&reset2,RISING);
  
  pinMode(A0,INPUT_PULLUP); //button 1
  pinMode(A1,INPUT_PULLUP); //button 2
  
  //shift register outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // load eeprom data into preset slots of sram
  for(int i=0;i<64;i++){
    if(EEPROM.read(i*8)!=255) {      //if eeprom value is anything other than 255 then it is a valid setting and will be loaded into SRAM
      setting[i][0]=EEPROM.read(i*8);                          
      setting[i][1]=EEPROM.read((i*8)+1);                  
      setting[i][2]=EEPROM.read((i*8)+2);                    
      setting[i][3]=EEPROM.read((i*8)+3); 
      setting[i][4]=EEPROM.read((i*8)+4); 
      setting[i][5]=EEPROM.read((i*8)+5); 
      setting[i][6]=EEPROM.read((i*8)+6);
      setting[i][7]=EEPROM.read((i*8)+7);
    }
    else {                    //if eepromTemp value is 255 then there is no value data for that preset and default values will be loaded
      setting[i][0]=0;             //default position 1 to 0 (step 1)
      setting[i][1]=0;             //default length 1 to 2 (4 steps)
      setting[i][2]=0;             //default position 2 to 0 (step 1)
      setting[i][3]=0;             //default length 2 to 2 (4 steps)
      setting[i][4]=0;
      setting[i][5]=0;
      setting[i][6]=0;
      setting[i][7]=0;
    }
  }
  zoomTemp=64;
} 




void loop()  // Main Dynamic Destiny Loop, calls functions
{
  MIDI.read();
  clock();
  
  if(millis()>50)
  buttons();
  
  switchLogic();
  
  if(millis()%5==0)
  LEDoutput();
}

