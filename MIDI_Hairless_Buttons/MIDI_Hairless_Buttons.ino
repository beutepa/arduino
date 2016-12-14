const int switchPin = 2;  // The switch is on Arduino pin 2
const int LEDpin = 13;     //  Indicator LED
 
 // Variables:
 byte note = 80;              // The MIDI note value to be played
 int lastNotePlayed = 0;     // note turned on when you press the switch
 int lastSwitchState = 0;    // state of the switch during previous time through the main loop
 int currentSwitchState = 0;
       // variable for reading the pushbutton status
byte noteON = 144;//note on command

void setup() {
  // initialize the LED pin as an output:
  pinMode(LEDpin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(switchPin, INPUT);

  Serial.begin(115200);
}

void loop() {
currentSwitchState = digitalRead(switchPin);
   // Check to see that the switch is pressed:
   if (currentSwitchState == 1) {
     //  check to see that the switch wasn't pressed last time
     //  through the main loop:
     if (lastSwitchState == 0) {
       // set the note value based on the analog value, plus a couple octaves:
       // note = note + 60;
       // start a note playing:
       MIDImessage(noteON, note, 100);//turn note on
       // save the note we played, so we can turn it off:
       lastNotePlayed = note;
       digitalWrite(LEDpin, HIGH);
     }
   }
   else {   // if the switch is not pressed:
     //  but the switch was pressed last time through the main loop:
     if (lastSwitchState == 1) {
       //  stop the last note played:
       MIDImessage(noteON, note, 0);
       digitalWrite(LEDpin, LOW);
     }
   }
 
   //  save the state of the switch for next time
   //  through the main loop:
   lastSwitchState = currentSwitchState;
 }

//send MIDI message
void MIDImessage(byte command, byte data1, byte data2) {
  Serial.write(command);
  Serial.write(data1);
  Serial.write(data2);
}
