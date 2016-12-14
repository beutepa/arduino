 /* 
 The circuit:
 Any analog input sensor is attached to analog in pin 0.

 created 2006
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe and Scott Fitzgerald

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Graph
 */

void setup() {
  // initialize the serial communication:
  Serial.begin(115200);
}

void loop() {
  // send the value of analog input 0:
  Serial.println(round(analogRead(A0)/4));
  // wait a bit for the analog-to-digital converter
  // to stabilize after the last reading:
  delay(200);
}
