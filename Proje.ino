#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <HCSR04.h>

const int in1 = D5;     // Motor sürücümüze bağladığımız pinleri tanımlıyoruz
const int in2 = D6;     
const int in3 = D7;
const int in4 = D8;

 WidgetLED on_far(V2);
 WidgetLED arka_far(V3);
 WidgetLED carpisma_uyarisi(V4);
 WidgetLED otomatik_frenleme(V5);

   
 
int LDRPin = A0;

int ses = D2;
int maviLed = D0;
int kirmiziLed = D1;


int trigPin =2;
int echoPin =0;

long zaman;
long mesafe;


char auth[] = "3yQ_jBtdAkEOuaG9bkYjcmlr-LGKT7tk";
char ssid[] = "";
char pass[] = "";

int minRange = 312;
int maxRange = 712;


void moveControl(int x, int y)
{
//Ileri
    if(y >= maxRange && x >= minRange && x<= maxRange)
    {
           // motor 1
           digitalWrite(in1, HIGH);
           digitalWrite(in2,  LOW);  
           // motor 2
           digitalWrite(in3, HIGH);
           digitalWrite(in4,  LOW);   
          
    }
//Geri
    if(y <= minRange && x >= minRange && x <= maxRange)
    {
            // motor 1
      digitalWrite(in1, LOW);
      digitalWrite(in2,  HIGH);  
       // motor 2
      digitalWrite(in3, LOW);
      digitalWrite(in4,  HIGH);

       digitalWrite(kirmiziLed, HIGH);
      
 
    }

    //Haraket etme
    if(y < maxRange && y > minRange && x < maxRange && x > minRange)
    {    
         digitalWrite(in1,  LOW);
         digitalWrite(in2,  LOW); 
         digitalWrite(in3,  LOW); 
         digitalWrite(in4,  LOW);

         digitalWrite(kirmiziLed, LOW);
       
 
    }

//Sağ
    if(x >= maxRange && y >= minRange && y<= maxRange)
    {
          digitalWrite(in1, HIGH);
          digitalWrite(in2,  LOW); 

          digitalWrite(in3, LOW);
          digitalWrite(in4,  LOW); 
        
    }
//Sol
    if(x <= minRange && y >= minRange && y <= maxRange)
    {
          digitalWrite(in1, LOW);
          digitalWrite(in2,  LOW); 

          digitalWrite(in3, HIGH);
          digitalWrite(in4,  LOW); 
    }
}


void setup()
{
 
    Serial.begin(9600);
    Blynk.begin(auth, ssid, pass);
    pinMode(in1, OUTPUT);  //Tüm pinlerden güç çıkışı olacağı için OUTPUT olarak ayarladık.
    pinMode(in2, OUTPUT);  
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);  
    
    pinMode(maviLed, OUTPUT);
    pinMode(kirmiziLed, OUTPUT);
    
    pinMode(trigPin, OUTPUT); 
    pinMode(echoPin,INPUT); 
    
     pinMode(ses, OUTPUT);
}


void loop()
{
  int isik_siddeti = analogRead(LDRPin);
  Serial.println(isik_siddeti);
  
 if(isik_siddeti<30)
 {
     digitalWrite(maviLed, HIGH);
     digitalWrite(kirmiziLed, HIGH);
   
     on_far.on();
     arka_far.on();
  
 }
 else
 {
   if (digitalRead(maviLed) == HIGH && digitalRead(kirmiziLed) == HIGH){
      digitalWrite(maviLed, LOW);
      digitalWrite(kirmiziLed, LOW);
      on_far.off();
      arka_far.off();  
   }

   
 }

  digitalWrite(trigPin, LOW); 
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  
  zaman = pulseIn(echoPin, HIGH);
  mesafe= (zaman /29.1)/2;    
  Serial.print("Uzaklik ");  
  Serial.print(mesafe);
  Serial.println(" cm");  
  Blynk.virtualWrite(V6,mesafe);
  if (mesafe<50 && mesafe>0){
     digitalWrite(ses, HIGH);
    carpisma_uyarisi.on();
    if (mesafe <40){
      otomatik_frenleme.on();
         digitalWrite(in1,  LOW);
         digitalWrite(in2,  LOW); 
         digitalWrite(in3,  LOW); 
         digitalWrite(in4,  LOW);

         delay(10000);
         
      }else {
        otomatik_frenleme.off(); 
     }
      
  }else {
     digitalWrite(ses, LOW);
     carpisma_uyarisi.off();
     otomatik_frenleme.off(); 
  }

   
    Blynk.run();
}

BLYNK_WRITE(V1)
{
       int x = param[0].asInt();
       int y = param[1].asInt();  
       moveControl(x,y);
     
}
