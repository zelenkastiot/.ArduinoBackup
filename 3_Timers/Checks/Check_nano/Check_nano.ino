#include <Servo.h>

//1st
#define trigPin1 3
#define echoPin1 2

Servo myServo1;

long duration, distance, Sensor1;

void setup() {
  //Servo for calculating 
  Serial.begin(9600);
  Serial.println("Serial begin");
  
  myServo1.attach(4); //servo1 - pin 8
  myServo1.write(0); //servo start position

  //Sensor1
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  
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
int i;
void loop() {
 
    for(i=45;i<=180;i+=45){
    SonarSensor(trigPin1, echoPin1);
    Serial.print(i/45);
    Serial.print("-> Distance: ");
    Serial.println(distance);
    myServo1.write(i);
    delay(100);
    }
    delay(1000);
    Serial.println();
    myServo1.write(0);
 }
