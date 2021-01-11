


int LED = 9; //led pin vo D9
int info = 0;//promenliva za informacijata koja ja dobivame od hc05
int sostojba = 0;//ke ni ja kazuva sostojbata dokolku veke upalena led da ne kazuva celo vreme (obratno istotaka)
bool flag = false; //false e ako led e izgasena a true e ako led e upalena

void setup() { 
  
  Serial.begin(9600);  //Pocnuvanje na serial comunication, za da mozeme da ja gledame distancata sto se meri
  
  //LED
  pinMode(LED, OUTPUT);    
  digitalWrite(LED, LOW);  
  //Heart sensor
  pinMode(10, INPUT); // Setup for leads off detection LO +
  pinMode(11, INPUT); // Setup for leads off detection LO -
  
}

void loop(){ 
    
  bluetooth();
  heartCheck();
  
}



void bluetooth() { 

  
  if(Serial.available() > 0){  //proveruva dali ima bilo kakva povratna informacija od serial liniite
    info = Serial.read();   //ako ima informacija (0 ili 1) od hc05 se smestuva vo info
    
    sostojba = 0;   //prethodnata sostojba ne se znae (ako e 1 ke se znae dali e upalena ili izgasena sijalicata)
  }
  if(info == '1'){                //ako dobie broj 1 
    digitalWrite(LED, HIGH);    //ke ja upali sijalicata
    if(sostojba == 0){              //ako sostojbata e 0, togas prikazi deka LED e upalena i potoa smeni ja vo 1
      Serial.println("LED ON");  //^^so sostojba promenlivata se pravi prevencija da ne pisuva celo vreme deka sijalicata e upalena
      sostojba = 1;
      flag = true;               //Flagot se koristi za dali e upalena ili izgasena sijalcata (true e za on, false e za off)
    }
   }
  else if(info == '0'){           //ako dobie broj 0
    digitalWrite(LED, LOW);      //ke ja izgasi sijalicata
    if(sostojba == 0){
      Serial.println("LED OFF"); //prikazuva LED OFF
      sostojba = 1;
      flag = false;              //izgasena sijalica
     }
  }
}

void heartCheck(){
 if((digitalRead(10) == 1)||(digitalRead(11) == 1)){
    Serial.println('!');
  }
  else{
    
      Serial.println(analogRead(A0));
  }
  //Wait for a bit to keep serial data from saturating
  delay(100);

}
