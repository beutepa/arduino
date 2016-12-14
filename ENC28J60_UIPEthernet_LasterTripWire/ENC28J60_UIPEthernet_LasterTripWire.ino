// ENC28J60
#include <UIPEthernet.h>

// LASERTRIPWIRE (LTW)
// Pin definitions
#define PIN_LASER_BLUE 4
#define PIN_LDR_BLUE 2

#define PIN_LASER_WHITE 5
#define PIN_LDR_WHITE 3

// Delay definitions 
#define DELAY_LASER 50
#define DELAY_BEEP 500

volatile boolean isBlueTriggered = false;
volatile boolean isWhiteTriggered = false;
boolean isEnabled = false;

int scoreBlue = 0;
int scoreWhite = 0;

// ENC28J60
EthernetServer server = EthernetServer(1000);

void setup()
{
  // LTW
  // Set up the laser and ldr pins
  pinMode(PIN_LASER_BLUE, OUTPUT);
  pinMode(PIN_LDR_BLUE, INPUT);

  pinMode(PIN_LASER_WHITE, OUTPUT);
  pinMode(PIN_LDR_WHITE, INPUT);
  
  // Use interrupts to process the beam being broken.
  attachInterrupt(digitalPinToInterrupt(PIN_LDR_BLUE), handleBlueInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_LDR_WHITE), handleWhiteInterrupt, RISING);
  
  // Serial comms will allow us to enable/disable the laser
  // Will be replaced with network communication if possible
  Serial.begin(9600);

  // ENC28J60
  uint8_t mac[6] = {0x00,0x01,0x02,0x03,0x04,0x05};
  IPAddress myIP(192,168,1,160);

  Ethernet.begin(mac,myIP);

  server.begin();
}

// LTW
void handleBlueInterrupt() {
  // Keep this function as short as possible.
  scoreBlue++;
  isBlueTriggered = true;
}

// LTW
void handleWhiteInterrupt() {
  // Keep this function as short as possible.
  scoreWhite++;
  isWhiteTriggered = true;
}

// Return score in json format, e.g.: {"score":{"blue":0,"white":2}}
String score() {
  String theScore = "{\"score\":{\"blue\":";
  theScore += scoreBlue;
  theScore += ",\"white\":" ;
  theScore += scoreWhite;
  theScore += "}}" ;
  return theScore;
}
  
void loop()
{
  if(isBlueTriggered && isEnabled) {
    // Disable laser
    digitalWrite(PIN_LASER_BLUE, LOW);
        
    // Who scored? b(lue) or w(hite)
    Serial.println("b"); // Is also returned through REST 
                         // (no specific url, parameters, just on every request)
    // delay(DELAY_BEEP); // Needed?
    
    // Enable laser
    digitalWrite(PIN_LASER_BLUE, HIGH);
    delay(DELAY_LASER);
    // Arm the system
    isBlueTriggered = false;
  }
  if(isWhiteTriggered && isEnabled) {
    // Disable laser
    digitalWrite(PIN_LASER_WHITE, LOW);
        
    // Who scored? b(lue) or w(hite)
    Serial.println("w"); // Will be replaced with network communication (REST)
    // delay(DELAY_BEEP); // Needed?
    
    // Enable laser
    digitalWrite(PIN_LASER_WHITE, HIGH);
    delay(DELAY_LASER);
    // Arm the system
    isWhiteTriggered = false;
  }

  while (Serial.available()){
    // Use serial to enable or disable the laser
    // Will be replaced with network communication if possible
    char tmp = Serial.read();
    switch(tmp) {
      // If lowercase 'e' (0x65) is recieved, enable it 
      case 'e':
        digitalWrite(PIN_LASER_BLUE, HIGH);
        digitalWrite(PIN_LASER_WHITE, HIGH);
        delay(DELAY_LASER);
        isEnabled = true;
        isBlueTriggered = false;
        Serial.println("Lasers enabled.");
        break;
      // If lowercase 'd' (0x64) is recieved, disable it
      case 'd':
        digitalWrite(PIN_LASER_BLUE, LOW);
        digitalWrite(PIN_LASER_WHITE, LOW);
        isEnabled = false;
        scoreBlue = 0;
        scoreWhite = 0;
        Serial.println("Lasers disabled.");
        break;
      // If lowercase 'r' (0x64) is recieved, reset the score
      case 'r':
        scoreBlue = 0;
        scoreWhite = 0;
        Serial.println("Score reset.");
        break;
    }
  }
  
  size_t size;

  if (EthernetClient client = server.available())
    {
      while((size = client.available()) > 0)
        {
          uint8_t* msg = (uint8_t*)malloc(size);
          size = client.read(msg,size);
          //FOR DEBUGGING Serial.write(msg,size);
          free(msg);
        }
      // The HTTP Response
      client.println("HTTP/1.1 200 OK");
      client.println("Access-Control-Allow-Origin: *");
      client.println("Server: Arduino");
      client.println("Content-Type: application/json");
      client.println("Pragma: no-cache");      
      client.println("");
      client.println(score());
      client.stop();
    }
}
