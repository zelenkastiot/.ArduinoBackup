#include <Servo.h>
Servo myservo1;
Servo myservo2;                      

/*
 * Код за две серва (отварање на канта) - Работи ок
 */
void setup() {
  Serial.begin(9600);
  Serial.println("Serial begin");
  
  myservo1.attach(3);
  myservo1.write(0);
  
  myservo2.attach(4);
  myservo2.write(180);
}

void loop() {

  myservo1.write(90);
  myservo2.write(90);
  delay(1000);
  Serial.println(1);
  delay(2000);
  myservo1.write(0);
  myservo2.write(180);
  delay(1000);
  Serial.println(0);

}
