
#include<Servo.h>

Servo servo;
int trig=4;
int echo=5;
int servoPin=8;
long distance,duration;
float cm;
int i;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(servoPin);
  servo.write(0);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
 

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
  for(i=45;i<=180;i+=45){
    SonarSensor(trig, echo);
    Serial.print("Distance: ");
    Serial.println(distance);
    servo.write(i);
    delay(3000);
  }
  

}



void SonarSensor(int trigPin,int echoPin)
{
digitalWrite(trigPin, LOW);
delayMicroseconds(20000);
digitalWrite(trigPin, HIGH);
delayMicroseconds(20000);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = (duration/2) / 29.1;
 
}
 
