#include "OneButton.h"
#include <LCD_I2C.h>  // Bibliothèque LCD I2C de blackhat
#define photoresistence  A0
#define JOYSTICK_X  A1
#define JOYSTICK_Y A2
#define BUTTON_PIN  2
#define LED_PIN  8
bool etatPhares = false;
unsigned long dernierTempsPhares = 0;
unsigned long  tempsPrecedent = 0;

typedef enum {
  TASK_OFF,
  TASK_ON,
} Tasks;

OneButton button(BUTTON_PIN, true, true);
Tasks currentTask = TASK_OFF;
LCD_I2C lcd(0x27, 16, 2);
byte custom85[8] = {
  0b11100,
  0b10100,
  0b11111,
  0b10100,
  0b11111,
  0b00001,
  0b00001,
  0b00111,
};
void allumage(){
 lcd.begin();
  lcd.backlight();
  lcd.createChar(0, custom85);
  lcd.setCursor(0, 0);
  lcd.print("ACHAMOU");
  lcd.setCursor(0, 1);
  lcd.write(byte(0));
  lcd.setCursor(14, 1);
  lcd.print("85");
  delay(3000);
  lcd.clear();
}
void setup() {
  pinMode(LED_PIN, OUTPUT);
  // Attacher la fonction qui s'exécutera
  // lorsqu'un clic sera détecter
  button.attachClick(myClickFunction);
  Serial.begin(115200);
  //Affichage au dermarage
  allumage();
 
}

void lectureAnalogique() {
  int valeur = analogRead(photoresistence);
  int pourcentageLuminosite = map(valeur, 0, 1023, 0, 100);
  lcd.setCursor(0, 0);
  lcd.print("Pct lum :");
  lcd.print(pourcentageLuminosite);
  lcd.print("%");
  int currentime = millis();
  if (pourcentageLuminosite < 50 && (currentime - dernierTempsPhares > 5000)) {
    etatPhares = true;
    digitalWrite(LED_PIN, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Phare : ON ");
  dernierTempsPhares = currentime ;
  } else if (pourcentageLuminosite > 50 && (currentime - dernierTempsPhares > 5000)) {

    etatPhares = false;
    digitalWrite(LED_PIN, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Phare : OFF");
    dernierTempsPhares = currentime ;
  }

}

void analogue() {
  int valY = analogRead(JOYSTICK_Y);
  int valX = analogRead(JOYSTICK_X);
  int vitesse = map(valY, 0, 1023, -120, 120);
  int degre = map(valX, 0, 1023, 90, -90);

  if (vitesse > 0) {
    lcd.setCursor(0, 1);
    lcd.print("Avance a ");
    lcd.print(vitesse);
    lcd.print(" km/h ");

  } else {
    lcd.setCursor(0, 1);
    lcd.print("Recule a ");
    lcd.print(vitesse);
    lcd.print("km/h ");
  }

  if (degre > 0) {
    lcd.setCursor(0, 0);
    lcd.print("Droite a ");
    lcd.print(degre);
    lcd.print("Deg ");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Gauche a ");
    lcd.print(degre);
    lcd.print("Deg ");
  }

}

  void turnOffTask() {
     analogue();
  }

  void turnOnTask() {
    lectureAnalogique();
  }

  void myClickFunction() {
  if (currentTask == TASK_OFF) {
    lcd.clear();
    currentTask = TASK_ON; 
  }
    else {
      lcd.clear();
      currentTask = TASK_OFF;
    }
      Serial.println(currentTask);
  
}
void affichage(){
    int valY = analogRead(JOYSTICK_Y);
    int valX = analogRead(JOYSTICK_X);
    Serial.print("etd:2349185,x:");
    Serial.print(valX);
    Serial.print(",y:");
    Serial.print(valY);
    Serial.print(",sys:");
    Serial.println(etatPhares ? 1 : 0);
 
}

void interchange(){
switch (currentTask) {
      case TASK_OFF:
        turnOffTask();
        break;
      case TASK_ON:
        turnOnTask();
        break;
    }
}
  void loop() {
    unsigned long currentTime = millis();
    if(currentTime - tempsPrecedent>=100){
      tempsPrecedent = currentTime;
       affichage();
    }
   
    button.tick();
    interchange();
   
     } 