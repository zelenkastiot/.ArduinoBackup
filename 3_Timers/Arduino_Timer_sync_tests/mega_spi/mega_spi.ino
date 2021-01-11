#include<SPI.h>

char niz[6]="Kirce";
int i;
int pinInt = 3;
void setup()
{
  pinMode(SS, OUTPUT);
  pinMode(pinInt,OUTPUT);
  digitalWrite(SS, HIGH);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV4);  //2^n = 2, 4, 8, 16, 32

}


void loop()
{
  digitalWrite(pinInt,LOW);
//  digitalWrite(SS, LOW);
//  for(i=0;i<6;i++){
//    SPI.transfer(niz[i]);
//    delay(1000);
//  }
//  SPI.transfer('\n');
//  digitalWrite(SS, HIGH);
  delay(15000);
  digitalWrite(pinInt,HIGH);
  delay(1000);
  
}
