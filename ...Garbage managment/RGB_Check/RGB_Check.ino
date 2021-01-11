/*
 * RGB led check 
 */

//RGB led -> 3 pins
int redPin = 9;
int greenPin = 10;
int bluePin = 11; 

void setup() {
  // put your setup code here, to run once:
  Serial.println("Color show");

  //RGB
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

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

void loop() {

 Serial.println("RED");
 setColor(255, 0, 0);
 delay(1000); //1 sec

 Serial.println("GREEN");
 setColor(0, 255, 0);
 delay(1000); //1 sec

 
 Serial.println("BLUE");
 setColor(0, 0, 255);
 delay(1000); //1 sec

 
 Serial.println("PURPLE");
 setColor(150, 0, 150);
 delay(1000); //1 sec
 
}
