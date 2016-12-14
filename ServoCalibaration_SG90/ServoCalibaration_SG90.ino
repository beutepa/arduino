// Sweep
// by BARRAGAN <http://barraganstudio.com>
// This example code is in the public domain.
 
 
#include <Servo.h>

#define SERVO_PIN 12
 
Servo myservo;  // create servo object to control a servo
                // a maximum of eight servo objects can be created
 
int pos = 0;    // variable to store the servo position
 
void setup()
{
  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
}
 
 
void loop()
{
    pos = 5;
    myservo.write(pos);
    delay(1000);                      
    pos = 43;
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(1000);                       // waits 15ms for the servo to reach the position    
    pos = 84;
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(1000);                       // waits 15ms for the servo to reach the position    
    pos = 126;
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(1000);                       // waits 15ms for the servo to reach the position    
    pos = 165;
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(1000);                       // waits 15ms for the servo to reach the position    
}
