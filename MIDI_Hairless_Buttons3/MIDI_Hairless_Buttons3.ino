const int LEDpin = 13;     //  Indicator LED

const byte BUTTON_COUNT = 6;
const int buttonPins[BUTTON_COUNT] = { 2, 3, 4, 5, 6, 7};
 
// MIDI Variables:
int note = 60;              // The MIDI note value to be played
int noteLDR = 60;
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

/**
 * OLED Variables/Functions/...
 */

#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);

const int WIDTH=128;
const int HEIGHT=64;
const int LENGTH=WIDTH;

const int analogInPin = A0;
const int analogLDRInPin = A1;
const int analogPiezoPin = A6;    // A6 => Piezo
int analogInValue = 0;
int analogLDRInValue = 0;
int analogPiezoValue = 0;

int x;
int y1[LENGTH];
int y2[LENGTH];
int y3[LENGTH];

void clearY1(){
  for(int i=0; i<LENGTH; i++){
    y1[i] = -1;
  }
}

void clearY2(){
  for(int i=0; i<LENGTH; i++){
    y2[i] = -1;
  }
}

void drawY1(){
  u8g.drawPixel(0, y1[0]);
  for(int i=1; i<LENGTH; i++){
    if(y1[i]!=-1){
      //u8g.drawPixel(i, y[i]);
      u8g.drawLine(i-1, y1[i-1], i, y1[i]);
    }else{
      break;
    }
  }
}

void drawY2(){
  u8g.drawPixel(0, y2[0]);
  for(int i=1; i<LENGTH; i++){
    if(y2[i]!=-1){
      //u8g.drawPixel(i, y[i]);
      u8g.drawLine(i-1, y2[i-1], i, y2[i]);
    }else{
      break;
    }
  }
}


void drawString(void) {
      
}
/** 
 * END OLED Variables/Functions/...
 */

void setup() {
  // initialize the LED pin as an output:
  pinMode(LEDpin, OUTPUT);

  // Initialize the buttons
  for (int thisButtonPin = 0; thisButtonPin < BUTTON_COUNT; thisButtonPin++) {
    pinMode(buttonPins[thisButtonPin], INPUT);
    //previousstate[thisButtonPin] == 0;
  }

  // Initialize the OLED
  x = 0;
  clearY1();
  clearY2();
  
  Serial.begin(115200);
}

void check_buttons(int AnalogInValue) {
  static byte previousstate[BUTTON_COUNT];
  static byte currentstate[BUTTON_COUNT];

  note = round(analogInValue/8);
  //note = round(analogRead(A1)/8);
  
  for (int index = 0; index < BUTTON_COUNT; index++) {
    currentstate[index] = digitalRead(buttonPins[index]);   // read the button
    //Serial.write(index + " - " + currentstate[index]);
    if (currentstate[index] == 1) {
      if (previousstate[index] == 0) {
        MIDImessage(noteON + index, note, 100);//turn note on
        digitalWrite(LEDpin, HIGH);
      }
    }
    else if (currentstate[index] == 0) {   // if the switch is not pressed:
       //  but the switch was pressed last time through the main loop:
       if (previousstate[index] == 1) {
         //  stop the last note played:
         MIDImessage(noteON + index, note, 0);
         digitalWrite(LEDpin, LOW);
       }
     }
     previousstate[index] = currentstate[index];
  }  
}

void loop() {
  analogInValue = analogRead(analogInPin);
  analogLDRInValue = analogRead(analogLDRInPin);
  analogPiezoValue = analogRead(analogPiezoPin);
  
  char POTbuf[4];
  itoa(analogInValue, POTbuf, 10);

  char LDRbuf[4];
  itoa(analogLDRInValue, LDRbuf, 10);

  char Piezobuf[4];
  itoa(analogPiezoValue, Piezobuf, 10);
  
  check_buttons(analogInValue);

  y1[x] = map(analogInValue, 0, 1023, HEIGHT-1, 0);;
  y2[x] = map(analogLDRInValue, 0, 1023, HEIGHT-1, 0);;

  u8g.firstPage();  
  do {
    drawY1();
    drawY2();
    u8g.setFont(u8g_font_6x10); u8g.drawStr(0, 11, POTbuf);
    u8g.setFont(u8g_font_6x10); u8g.drawStr(30, 11, LDRbuf);
    u8g.setFont(u8g_font_6x10); u8g.drawStr(60, 11, Piezobuf);
    
  } while( u8g.nextPage() );
  
  //delay(10);

  x++;
  if(x >= WIDTH){
    x = 0;
    clearY1();
    clearY2();
  }
}

//send MIDI message
void MIDImessage(byte command, byte data1, byte data2) {
  Serial.write(command);
  Serial.write(data1);
  Serial.write(data2);
}
