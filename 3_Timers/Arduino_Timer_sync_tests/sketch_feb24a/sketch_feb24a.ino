
#include <SPI.h>
#include <Servo.h>  
//1st
#define trigPin1 10
#define echoPin1 9

//Flame detector setup
int flame_sensor = 47;
int flame_detected;
int state = 0;//simple variable for displaying the state

#define BUZZER 52 //Buzzer

Servo myServo2; //Servo 2

long duration, distance, Sensor1;
Servo myServo1;
int count = 1;

void setup() {
  Serial.begin(9600);   // Initiate a serial communication
  
  myServo1.attach(8); //servo1 - pin 8
  myServo1.write(90); //servo start position

  
  myServo2.attach(46);
  myServo2.write(0);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);

  pinMode(flame_sensor, INPUT);


}

void loop() {

  flame();
  
//  if(count==1){
//    count=0;
//    myServo1.write(180);
//  }
//
//  else{
//    count=1;
//    myServo1.write(0);
//  }
//  
//
// 
//  SonarSensor(trigPin1, echoPin1);
//  Sensor1 = distance;
//
//
//  Serial.println(Sensor1);
//  delay(2000);
//  myServo1.write(90);  
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

void flame(){
  
  flame_detected = digitalRead(flame_sensor);
  if (flame_detected == 1)
  {
    if(state==1){
    Serial.println("Flame detected...! take action immediately.");
    tone(BUZZER, 300);
    
    myServo2.write(90);
    state=0;
    }
    
  }
  else
  {
    
    myServo2.write(0);
    noTone(BUZZER);
    delay(200);
    if(state==0){
    state=1;
    }
  }
  delay(100);
  }
