//.....////////////////////////////////////////////////////////////.....//
//.....////////////////////////////////////////////////////////////.....//
//.....//      Macro Machines Level Logistics Copyright 2014     //.....//
//.....//      by Benjamin Davis and Nicholas C. Raftis III      //.....//
//.....////////////////////////////////////////////////////////////.....//
//.....////////////////////////////////////////////////////////////.....//

// INCLUDES

#include <PinChangeInt.h>
#include <Wire.h>


// GLOBAL IO VARIABLES

#define buttonEncoder B00000001
#define buttonMutate B00000010
#define buttonLength B00000100
#define buttonGlide B00001000
#define buttonGate B00010000
#define buttonRepeat B00100000
#define buttonRotate B01000000
byte buttons;

byte buttonOld;
byte buttonNew;
byte gateOld;
byte gateNew;

boolean clockInput[2];
boolean resetInput[2];
unsigned long resetTime;
boolean mutateGate[2]; // mutate jack

boolean gateOut;

boolean LEDbargraphMode = 0;
unsigned long bargraphCountDown = millis();
byte currentAdjustmentValue;

byte currentStep;

byte LEDlevel[8];
byte LEDscaler[16] = {0, 0, 1, 1, 2, 3, 4, 5, 12, 16, 23, 33, 48, 56, 62, 88};
//byte LEDscaler[16] = {0, 1, 2, 3, 5, 8, 12, 20, 32, 52, 74, 80, 90, 100, 110, 120};

//  STORED AND LOADED FROM FRAM [voltage/step number] [0 = current / 1 = morph destination] 

int voltages[8] = {random(256),random(256),random(256),random(256),random(256),random(256),random(256),random(256)};    // voltages for dacs and sequencer
int rotate[8] = {random(8),random(8),random(8),random(8),random(8),random(8),random(8),random(8)};                  // shifts 8 voltage outs 
int repeat[8] = {random(8),random(8),random(8),random(8),random(8),random(8),random(8),random(8)};                  // repeats backward or forward n number of steps   0-7 (backward) 8-15 (forward)                                        
int length[8]= {random(8),random(8),random(8),random(8),random(8),random(8),random(8),random(8)};                  // clock divider counts n pulses until advancing 0-8
int gate[8] = {random(8),random(8),random(8),random(8),random(8),random(8),random(8),random(8)};                   // length of gate off-full on
int glide[8] = {random(8),random(8),random(8),random(8),random(8),random(8),random(8),random(8)};                // sets the 8 bit pot for analogue slew 0-8
                      
int mutateDestination[8] = {random(256),random(256),random(256),random(256),random(256),random(256),random(256),random(256)};   // 1 - 8 mutate 1-8 values, above that mutate all
byte mutateAmount = 1;      // Depth of Mutation 
int mutateCV;
int mutateTemp[8];         // Temporary storage of shifting mutate values
  
int morphAmount;
int morphTemp[8];



// ENCODER STUFF

#include <Encoder.h>
Encoder knob(3,2);                 // set pins for encoder
long oldPosition;                  // Used to check direction of encoder
long newPosition;
unsigned long changeTime;          // Last time encoder was moved      
byte selectedSetting = 0;          // indicates which value is being selected and manipulated


//  SHIFT PWM STUFF

const int ShiftPWM_latchPin=8;      
const bool ShiftPWM_invertOutputs = false;
const bool ShiftPWM_balanceLoad = false;

#include <ShiftPWM.h>                       // include ShiftPWM.h after setting the pins!

byte maxBrightness = 255;
unsigned char pwmFrequency = 60;
unsigned int numRegisters = 1;
unsigned int numOutputs = numRegisters*8;
//unsigned int numRGBLeds = numRegisters*8/3;
unsigned int fadingMode = 0;               // start with all LED's off.
unsigned long startTime = 0;               // start time for the chosen fading mode


// MIDI Bus

#include <MIDI.h>
byte activeSetting;                        // the ACTIVE SETTING NUMBER!!!
byte zoom = 64;                            // cc #16 value (64 = off), (0 = zoom out / bank / alt function), (127 = zoom in / save), (33 = save all)




///////////////////////////////////////   SETUP    ///////////////////////////////////////

void setup()
{
  Wire.begin(); 
  TWBR = 12; // set i2c to 400khz
  //IO EXPAND SETUP
  IORegister(0x00,255); //io dir a
  IORegister(0x01,255); //io dir b
  IORegister(0x0C,255); //pullup a
  IORegister(0x0D,B00001000); //pullup b
  IORegister(0x02,255); // polarity a
  IORegister(0x03,B00001000); // polarity b
  IORegister(0x04,B01111111); // GPINTEN A
  IORegister(0x05,B00001111); // GPINTEN B
  IORegister(0x06,0); // DEFVAL A
  IORegister(0x07,0); // DEFVAL B
  IORegister(0x08, 0); // INTCON A
  IORegister(0x09, 0); // INTCON B
  IORegister(0x0A, B00011010); // IOCON A
  IORegister(0x0B, B00011010); // IOCON B
  //end IO EXPAND SETUP
  
  MIDI.begin(1);
  MIDI.setHandleProgramChange(handleProgramChange);
  MIDI.setHandleControlChange(handleControlChange);
  
  pinMode(A1,INPUT); // MCP23017 interrupt 1    buttons change states
  pinMode(A2,INPUT); // MCP23017 interrupt 2    jacks inputs or digital trim change

  pinMode(5,OUTPUT); // LDAC for dac1
  pinMode(6,OUTPUT); // LDAC for dac2
  pinMode(7,OUTPUT); // Encoder button??????
  pinMode(9,OUTPUT); // mutate CV PWM (replace with direct connection to led, add resistor on CV in)
  
  digitalWrite(7,LOW);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  
  ShiftPWM.SetAmountOfRegisters(numRegisters);  
  ShiftPWM.Start(pwmFrequency,maxBrightness);
  
  //for(byte i = 0; i < 255; i++)
    //LEDscaler[i] = log
  
  //PCintPort::attachInterrupt(A2,&sequencer,HIGH);
  //PCintPort::attachInterrupt(A1,&checkButtons,HIGH);
  //Serial.begin(115200);
  //  Serial.println("hello world");
}







///////////////////////////////////////   MAIN LOOP    ////////////////////////////////////////
//__________________________________________________________________________________________//

void loop()
{ 
  ////////////////////// INPUT ////////////////////
 
  MIDI.read();
  
  buttonNew = bitRead(PINC,1);
  gateNew = bitRead(PINC,2);
  
  newPosition = knob.read();

  
  if ((newPosition > oldPosition+3)||(newPosition < oldPosition-3)) 
  {
    handleEncoder();
  }
  
  if(buttonNew) // buttons changed states
  {
    buttons=readInput(0);
  }
  

 if(gateNew) // jacks inputs or digital trim changed
  { 
    byte tempIntFlag=intFlag(1);
    checkGates();
    
    if(bitRead(tempIntFlag,0))
    {
      setMutateDestination();
    }
      if (bitRead(tempIntFlag,1))
      {
        resetSeq();
      }
     else if(bitRead(tempIntFlag,2))
     {
        sequencer();
     }
  }

  

  ////////////////////// CALCULATE //////////////////// 
  
  mutate();
  
  ////////////////////// OUTPUT ////////////////////
  setSlew(glide[currentStep]);               // 0 = none, 10 = glide, 20-128 = slow
  sequenceOut(mutateTemp[currentStep]<<4);     // sequence CV output with slew
  voltageOutputs();
  LEDout();
}



 




