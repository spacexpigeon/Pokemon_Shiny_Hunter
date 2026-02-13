#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// --- TWOJE USTAWIENIA ---
#define S0_GORA 150
#define S0_DOL  475
#define S1_GORA 400
#define S1_DOL  150

// Funkcja podtrzymująca (zostawiamy, bo działa)
void czekajZPodtrzymaniem(int sekundy) {
  for (int i = 0; i < sekundy; i++) {
    pwm.setPWM(0, 0, S0_GORA); 
    pwm.setPWM(1, 0, S1_GORA);
    delay(1000); 
  }
}

void setup() {
  Serial.begin(115200); // Komunikacja z PC
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

  // --- SERIA KLIKNIĘĆ PRZYCISKU A ---
  
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
  
  // --- CZAS NA ZAŁADOWANIE WALKI ---
  Serial.println("Czekam na walkę (wydłużony czas)...");
  
  // ZMIANA: Było 14, jest 16. To daje +2 sekundy zapasu.
  czekajZPodtrzymaniem(16); 

  // --- KOMUNIKACJA Z PC ---
  
  // 1. Wyczyść bufor
  while(Serial.available() > 0) { Serial.read(); }

  // 2. Wyślij sygnał do Pythona
  Serial.println("SKANUJ"); 

  // 3. Czekaj na odpowiedź z komputera
  Serial.println("Czekam na decyzję PC...");
  
  bool decyzjaOdebrana = false;
  
  while (decyzjaOdebrana == false) {
    if (Serial.available() > 0) {
      char komenda = Serial.read();
      
      if (komenda == 'r') {
        // --- SCENARIUSZ 1: ZWYKŁY HO-OH (RESET) ---
        Serial.println("Otrzymano RESET - to zwykły Pokemon.");
        
        // Wykonaj reset
        pwm.setPWM(1, 0, S1_DOL); 
        delay(600); 
        pwm.setPWM(1, 0, S1_GORA); 
        
        // Czekanie po resecie
        czekajZPodtrzymaniem(15); 
        decyzjaOdebrana = true; 
      }
      else if (komenda == 's') {
        // --- SCENARIUSZ 2: SHINY! (STOP) ---
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