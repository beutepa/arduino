/*


 */
#include <SoftwareSerial.h>

SoftwareSerial esp8266(2, 3); // Make RX Arduino pin 2 and TX Arduino pin 3
                              // Meaning: 
                              //        TX(esp8266) -> p2(arduino)
                              //        RX(esp8266) -> p3(arduino)

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // set the data rate for the SoftwareSerial port
  esp8266.begin(9600);
}

void loop() { // run over and over
  if (esp8266.available()) {
    while(esp8266.read()) {
      char c = esp8266.read();
      Serial.write(c);
    }
  }
  if (Serial.available()) {
    delay(1000);
    String command="";
    
    while(Serial.available()) {
      command+=(char)Serial.read();
    }
    esp8266.println(command);
  }
}

