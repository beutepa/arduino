int LDR_PIN = 1;
int POT_PIN = 0;
int INTERNAL_LED_PIN = 13;
int LDR_Resistance = 1000;
int TRESHOLD = 600;

void setup() {
  Serial.begin(9600);
}

void loop(){
  Serial.print(analogRead(LDR_PIN));
  Serial.print(",");
  Serial.println(analogRead(POT_PIN));
}
