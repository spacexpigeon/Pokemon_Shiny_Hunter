#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


#define S0_GORA 150
#define S0_DOL  475
#define S1_GORA 400
#define S1_DOL  150


void czekajZPodtrzymaniem(int sekundy) {
  for (int i = 0; i < sekundy; i++) {
    pwm.setPWM(0, 0, S0_GORA); 
    pwm.setPWM(1, 0, S1_GORA);
    delay(1000); 
  }
}

void setup() {
  Serial.begin(115200); 
  pwm.begin();
  pwm.setPWMFreq(60);

  pwm.setPWM(0, 0, S0_GORA);
  pwm.setPWM(1, 0, S1_GORA);
  
  Serial.println("System gotowy. Czekam na start...");
  delay(2000);
}

void loop() {
  
  Serial.println("START_SEKWENCJI"); 
  czekajZPodtrzymaniem(6); 


  
  Serial.println("KLIK 1");
  pwm.setPWM(0, 0, S0_DOL); delay(500); pwm.setPWM(0, 0, S0_GORA);
  czekajZPodtrzymaniem(4); 

  Serial.println("KLIK 2");
  pwm.setPWM(0, 0, S0_DOL); delay(500); pwm.setPWM(0, 0, S0_GORA);
  czekajZPodtrzymaniem(5); 

  Serial.println("KLIK 3");
  pwm.setPWM(0, 0, S0_DOL); delay(500); pwm.setPWM(0, 0, S0_GORA);
  czekajZPodtrzymaniem(5); 

  Serial.println("KLIK 4");
  pwm.setPWM(0, 0, S0_DOL); delay(500); pwm.setPWM(0, 0, S0_GORA);
  czekajZPodtrzymaniem(3); 

  Serial.println("KLIK 5");
  pwm.setPWM(0, 0, S0_DOL); delay(500); pwm.setPWM(0, 0, S0_GORA);
  

  Serial.println("Czekam na walkę (wydłużony czas)...");
 
  czekajZPodtrzymaniem(16); 


  while(Serial.available() > 0) { Serial.read(); }


  Serial.println("SKANUJ"); 

  Serial.println("Czekam na decyzję PC...");
  
  bool decyzjaOdebrana = false;
  
  while (decyzjaOdebrana == false) {
    if (Serial.available() > 0) {
      char komenda = Serial.read();
      
      if (komenda == 'r') {

        Serial.println("Otrzymano RESET - to zwykły Pokemon.");
        

        pwm.setPWM(1, 0, S1_DOL); 
        delay(600); 
        pwm.setPWM(1, 0, S1_GORA); 
        

        czekajZPodtrzymaniem(15); 
        decyzjaOdebrana = true; 
      }
      else if (komenda == 's') {

        Serial.println("SHINY!!! ZATRZYMUJĘ WSZYSTKO!");
        while(true) {
          pwm.setPWM(0, 0, S0_GORA);
          pwm.setPWM(1, 0, S1_GORA);
          delay(1000);
        }
      }
    }
    delay(50);
  }
}