byte noteON = 144;//note on command

void setup() {
  Serial.begin(115200);
}

void loop() {
  for (byte note=50;note<80;note++) {//from note 50 (D3) to note 79 (G5)
    MIDImessage(noteON, note, 100);//turn note on
    delay(300);//hold note for 300ms
    MIDImessage(noteON, note, 0);//turn note off (note on with velocity 0)
    delay(200);//wait 200ms until triggering next note
  }
}

//send MIDI message
void MIDImessage(byte command, byte data1, byte data2) {
  Serial.write(command);
  Serial.write(data1);
  Serial.write(data2);
}
