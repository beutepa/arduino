int LASER_PIN = 2;
int LDR_PIN = 5;
int INTERNAL_LED_PIN = 13;
int LDR_Resistance = 1000;
int TRESHOLD = 600;

void setup() {
  pinMode(INTERNAL_LED_PIN, OUTPUT);
  pinMode(LASER_PIN, OUTPUT);
  Serial.begin(9600);
  digitalWrite(LASER_PIN, HIGH);
}

void loop(){
  LDR_Resistance = analogRead(LDR_PIN);
  Serial.println(LDR_Resistance);
  
  //if (LDR_Resistance < TRESHOLD) {
  //  Serial.println("opa"); 
  //  digitalWrite(INTERNAL_LED_PIN, HIGH);
  //} else {
  //  Serial.println("oma");  
  //  digitalWrite(INTERNAL_LED_PIN, LOW);
  //}
}
