
#include <LiquidCrystal.h>

// moisture sensor variables
const int moistureSensorPin = A0;
const int minValue = 0;
const int maxValue = 1023;

// pH sensor variables
const int pHsensorPin = A1;
const int pHcalibrationValue = 7.0;
const int pHcalibrationVoltage = 2.5; 

// lcd interfacing - lcd(Rs, E, D4, D5, D6, D7);
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); 

}

bool toRepeat = true;

void loop() {

  if(toRepeat == true) {
    loading();
    toRepeat = false;
  }
  else {
    fetching();
  }


  delay(300);
  lcd.clear();

  moistureSensor();

  delay(500);
  pHsensor();

  delay(500);
  temperature();

  delay(500);
  lcd.clear();
}

// loading text in display
void loading() {
  lcd.print("Loading");
  delay(100);
  lcd.clear();
  lcd.print("Loading.");
  delay(100);
  lcd.clear();
  lcd.print("Loading..");
  delay(100);
  lcd.clear();
  lcd.print("Loading...");
}

// fetching text display
void fetching() {
  lcd.print("Please wait");
}

// code for pH value sensor
void pHsensor() {
  int pHvalue = analogRead(pHsensorPin);

  // analog value to voltage
  float voltage = pHvalue * (5.0 / 1023.0);

  float pH = pHcalibrationValue + ((voltage - pHcalibrationVoltage) * (-5.0 / 1.65));

  Serial.print("pH Value : ");
  Serial.println(pH, 2);

  lcd.setCursor(0, 1);

  lcd.print("pH:");
  lcd.print(pH, 2);
  
}

// code for soil moisture sensor
void moistureSensor() {
  int moisture = analogRead(moistureSensorPin);
  int moisturePercent = map(moisture, minValue, maxValue, 0, 100);

  Serial.print("Moisture Percentage : ");
  Serial.print(moisturePercent);
  Serial.println("%");
  
  lcd.setCursor(0, 0);
  lcd.print("moisture:");
  lcd.print(moisturePercent);
  lcd.print("%");
}

// demo code for temperature
void temperature() {
  int temp = 20;
  lcd.setCursor(0, 1);
  lcd.print("temp(c):");
  lcd.print(temp);

  Serial.print("Temperature : ");
  Serial.print(temp);
  Serial.println(" C");
}


