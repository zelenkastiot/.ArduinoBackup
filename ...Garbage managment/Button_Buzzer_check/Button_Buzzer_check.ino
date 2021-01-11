/*
 * Check for button and buzzer 
 */


#define BUTTON1 3
#define BUTTON2 4
#define BUZZER 12
 
void setup() {
 Serial.begin(9600);
 //Button 1
 pinMode(BUTTON1,INPUT);
 //Button 2
 pinMode(BUTTON2,INPUT);
 //Buzzer
 pinMode(BUZZER, OUTPUT);
 noTone(BUZZER);
}

void loop() {
  if((digitalRead(BUTTON1) == LOW)||(digitalRead(BUTTON2) == LOW)){
    Serial.println("Pressure plate activated");
    tone(BUZZER, 600);
    delay(500);
       
  }else{
    noTone(BUZZER);
  }

}
