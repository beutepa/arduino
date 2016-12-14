// Pin definitions
#define PIN_LASER_BLUE 4
#define PIN_LDR_BLUE 2

#define PIN_LASER_WHITE 5
#define PIN_LDR_WHITE 3

// Delay definitions 
#define DELAY_LASER 50
#define DELAY_BEEP 500

int i = 0;

volatile boolean isBlueTriggered = false;
volatile boolean isWhiteTriggered = false;
boolean isEnabled = false;

void setup() {
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
}

void handleBlueInterrupt() {
  // Keep this function as short as possible.
  isBlueTriggered = true;
}

void handleWhiteInterrupt() {
  // Keep this function as short as possible.
  isWhiteTriggered = true;
}

void loop() {
  if(isBlueTriggered && isEnabled) {
    // Disable laser
    digitalWrite(PIN_LASER_BLUE, LOW);
        
    // Who scored? b(lue) or w(hite)
    Serial.println("b"); // Will be replaced with network communication (REST)
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
        break;
      // If lowercase 'd' (0x64) is recieved, disable it
      case 'd':
        digitalWrite(PIN_LASER_BLUE, LOW);
        digitalWrite(PIN_LASER_WHITE, LOW);
        isEnabled = false;
        break;
    }
  }
}
