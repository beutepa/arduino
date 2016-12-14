
#include <Button.h>


// MIDI Variables:
byte note = 60;              // The MIDI note value to be played

byte noteON = 144;//0x90
byte noteOFF = 128; //0x80
/**
 MIDI commands

   0x80     Note Off
   0x90     Note On
   0xA0     Aftertouch
   0xB0     Continuous controller
   0xC0     Patch change
   0xD0     Channel Pressure
   0xE0     Pitch bend
   0xF0     (non-musical commands)
**/

const byte BUTTON_COUNT = 3;
const int buttonPins[BUTTON_COUNT] = { 2, 3, 4};
Button button[BUTTON_COUNT] = {Button(2,PULLDOWN), Button(3,PULLDOWN), Button(4,PULLDOWN) };

void setup(){
  pinMode(13,OUTPUT); //debug to led 13
  Serial.begin(115200);
}

void loop(){

  for (int index = 0; index < BUTTON_COUNT; index++) {
    if(button[index].isPressed()){
    digitalWrite(13,HIGH);
    Serial.println(index);
    }else{
    digitalWrite(13,LOW);
    }
  }
}

