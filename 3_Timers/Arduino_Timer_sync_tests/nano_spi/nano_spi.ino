#include<SPI.h>
#include<LowPower.h>

char buf[9];
volatile byte pos = 0;
volatile boolean process_it = false; 


void setup()
{
  Serial.begin(9600);
  Serial.println("SPI test");
//  pinMode(MISO, OUTPUT);
//  digitalWrite(MISO, LOW);
//  SPCR |= _BV(SPE); //SPI Enable
  //SPCR |= _BV(SPIE);
  //SPI.attachInterrupt();
  pinMode(2,INPUT);
attachInterrupt(digitalPinToInterrupt(2),wakeUp,LOW);
delay(2000);
  
}


void wakeUp(){
process_it=true;  
}

//ISR(SPI_STC_vect)
//{
//  byte c = SPDR;
//
//  if(pos < sizeof buf)
//  {
//    buf[pos++] = c;
//    if(c == '\n')
//     {
//        process_it = true;
//}
//} 
//}

void loop()
{
//    if(process_it)
//      {
//        Serial.println(buf);
//        pos = 0;
//        process_it = false;
//}

      Serial.println("Go to sleep");
      Serial.flush();
      
      
      LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_ON); 
      delay(100);
      if(process_it){
        
        Serial.println("Awake");
        
        process_it=false;
        } 
      delay(100);
}
