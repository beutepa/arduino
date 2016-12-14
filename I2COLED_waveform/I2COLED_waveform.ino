// ref u8glib: https://code.google.com/p/u8glib/
// To install u8glib on Arduino IDE: http://goo.gl/j3olBA
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);

const int WIDTH=128;
const int HEIGHT=64;
const int LENGTH=WIDTH;

const int analogInPin = A0;
int analogInValue = 0;

int x;
int y[LENGTH];

void clearY(){
  for(int i=0; i<LENGTH; i++){
    y[i] = -1;
  }
}

void drawY(){
  u8g.drawPixel(0, y[0]);
  for(int i=1; i<LENGTH; i++){
    if(y[i]!=-1){
      //u8g.drawPixel(i, y[i]);
      u8g.drawLine(i-1, y[i-1], i, y[i]);
    }else{
      break;
    }
  }
}

void setup(void) {
  x = 0;
  clearY();
}

void loop(void) {
  
  analogInValue = analogRead(analogInPin);
  
  y[x] = map(analogInValue, 0, 1023, HEIGHT-1, 0);;

  u8g.firstPage();  
  do {
    drawY();
  } while( u8g.nextPage() );
  
  //delay(10);

  x++;
  if(x >= WIDTH){
    x = 0;
    clearY();
  }
}
