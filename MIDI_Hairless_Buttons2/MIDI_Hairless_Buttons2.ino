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
  
void setup() {
  // initialize the LED pin as an output:
  pinMode(LEDpin, OUTPUT);

  for (int thisButtonPin = 0; thisButtonPin < BUTTON_COUNT; thisButtonPin++) {
    pinMode(buttonPins[thisButtonPin], INPUT);
    //previousstate[thisButtonPin] == 0;
  }

  Serial.begin(115200);
}
void check_buttons() {
  static byte previousstate[BUTTON_COUNT];
  static byte currentstate[BUTTON_COUNT];

  note = round(analogRead(A0)/8);
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
  
  check_buttons();
}

//send MIDI message
void MIDImessage(byte command, byte data1, byte data2) {
  Serial.write(command);
  Serial.write(data1);
  Serial.write(data2);
}
