/* 2019
 * Authors: Kiril & Filip 
 * Project: Garbage managment system 
 * Mentor: prof. Igor Miskovski 
 * 
 * Our system has a purpose to calculate the volume of a container using ultra sonic sensors and sevral math equatations. 
 * The volume is than send with the Feather M0 to the gateway closesest to the garbage. The gateway is a part of The Things Network.   
 * The same message is than processed and returned for the user to see the state of the garbage container. 
 * The idea is to create a larger network of this containers for easier monitoring and keeping the inviorment free. 
 * The is just a prototype. 
 * 
 *The whole system is divided into 5 sub-systems:  
 *  1. Volume sub-system [4 x HC-S04 ultra sonic sensors]
 *  2. Opening sub-system [1 x RGB led,1 x Servo, 1 x RFID, 1 x Pressure plate (4 x buttons), 1 x Fire sensor]
 *  3. Time-tracking sub-system [1 x DS3231]
 *  4. Lora comunication sub-system [1 x Feather M0 (RF96 - 868~900 MHz), 1 X Antena]
 *  5. Charging sub-system [1 x Power Bank (12 000 mAh)or direct electricity using a charger for the mega]
 *
 * RFID 7 wires: 
 *  Black wire - D53 (SDA)
 *  White wire - D52 (SCA)
 *  Blue wire - D51 (MOSI)
 *  Orange wire - D50 (MISO)
 *  Brown wire - GND (GND)
 *  Yellow wire - D49 (Reset)
 *  Red wire - 3.3 V (Vcc/3.3V) 
 *  
 * Fire sensor 1 wire: 
 *  Red wire - 5 V (Vcc/5V)  
 *  Brown wire - GND (GND)
 *  Orange wire - D5 (Signal)
 *  
 * Buzzer 2 wires:
 *  White wire - GND (Short)
 *  Yellow wire - D12 (Long)
 */


#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

//RGB led -> 3 pins
int redPin = 9;
int greenPin = 10;
int bluePin = 11; 

//1st
#define trigPin1 13
#define echoPin1 12

//2nd
#define trigPin2 11
#define echoPin2 10

//3rd
#define trigPin3 9
#define echoPin3 8

//4th
#define trigPin4 33
#define echoPin4 35

unsigned long current;
unsigned long lastOccur=0; 
long duration, distance, Sensor1,Sensor2,Sensor3,Sensor4;
long sensorAverage;
long capacity = 100; 

#define BUZZER 12 //Buzzer

int BUTTON1 = 27; //B1 for pressure plate
int BUTTON2 = 6; //B2 for pressure plate

MFRC522 mfrc522(53, 49);   // MFRC522 instance

Servo myServo1; //Servo 1
Servo myServo2; //Servo 2 


//Flame detector setup
int flame_sensor = 5;
int flame_detected;
int state = 0;//simple variable for displaying the state

void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  //Servo for calculating 
  myServo1.attach(7); //servo1 - pin 8
  myServo1.write(90); //servo start position

  //Servo for opening 
  myServo2.attach(3);
  myServo2.write(180);

  //RGB
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  //Buzzer
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);

  //Button 1 and 2
  pinMode(BUTTON1,INPUT);
  pinMode(BUTTON2,INPUT);

  //Flame
  pinMode(flame_sensor, INPUT);

  //Sensor1
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  //Sensor2
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  //Sensor3
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  //Sensor4
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);
  
  Serial.println("Put your card to the reader...");
  Serial.println();

}

void loop() 
{
  current=millis();
  flame();
  pressure_plate();
  rfid();

  if(current - lastOccur >= 20000){
  SonarSensor(trigPin1, echoPin1);
  Sensor1 = distance;
  
  SonarSensor(trigPin2, echoPin2);
  Sensor2 = distance;
  
  SonarSensor(trigPin3, echoPin3);
  Sensor3 = distance;
  
  SonarSensor(trigPin4, echoPin4);
  Sensor4 = distance; 
  
    
  calculate(Sensor1,Sensor2,Sensor3,Sensor4);
  lastOccur = millis();
  }
}

void calculate(long Sensor1, long Sensor2, long Sensor3, long Sensor4){


  sensorAverage = (Sensor1 + Sensor2 + Sensor3 + Sensor4)/4; 
//Serial.println(sensorAverage);
//delay(200);  

  int procent = (sensorAverage*100)/capacity;
  Serial.print("Garbage procent: ");
  Serial.print(100-procent);
  Serial.println(" %");
  delay(300);
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

//FLAME

void flame(){
  
  flame_detected = digitalRead(flame_sensor);
  if (flame_detected == 1)
  {
    if(state==1){
    Serial.println("Flame detected...! take action immediately.");
    tone(BUZZER, 300);
    myServo1.write(180);
    myServo2.write(90);
    setColor(255, 0, 0);
    state=0;
    }
    
  }
  else
  {
    myServo1.write(90);
    myServo2.write(180);
    noTone(BUZZER);
    delay(200);
    setColor(0, 0, 0);
    if(state==0){
    state=1;
    }
  }
  delay(100);
  }


//PRESSURE PLATE 

void pressure_plate(){
if((digitalRead(BUTTON1) == HIGH)||(digitalRead(BUTTON2) == HIGH)){
    
    Serial.println("Pressure plate activated");
    delay(500);
    setColor(0, 255, 0);
    tone(BUZZER, 600);
    delay(300);
    noTone(BUZZER);
    myServo1.write(90);
    myServo2.write(90);
    delay(5000);
    myServo1.write(0);
    myServo2.write(180);
    setColor(0, 0, 0);;
    }
}


//RFID 

void rfid(){
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
 
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
 
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  if (content.substring(1) == "50 80 73 A5") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    setColor(0, 255, 0); //Green light
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    myServo1.write(180);
    myServo2.write(90);
    delay(5000);
    myServo1.write(90);
    myServo2.write(180);
    setColor(0, 0, 0); //Green light off
  }
 
 else   {
    Serial.println(" Access denied");
    setColor(255, 0, 0); //Red light
    tone(BUZZER, 300);
    delay(1000);
    setColor(0, 0, 0); //Red light off
    noTone(BUZZER);
  }
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
