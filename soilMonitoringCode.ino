
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);

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

//temperature sensor setup
float RawValue=0;
float RawHigh = 88.62;
float RawLow = 4.56;
float ReferenceHigh = 99.99;
float ReferenceLow = 0;
float RawRange = RawHigh - RawLow;
float ReferenceRange = ReferenceHigh - ReferenceLow;

const int numReadings = 5;  // Number of readings to average
int readings[numReadings];  // Array to store the sensor readings
int readIndex = 0;  // Index for the current reading
long total = 0;  // Running total of readings
float average = 0.0;  // Calculated average


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); 
   for(int i=0; i<numReadings; i++){
    readings[i]=0;
  } 

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
  int pHvalue = ***ogRead(pHsensorPin);

  // ***og value to voltage
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
  int moisture = ***ogRead(moistureSensorPin);
  int moisturePercent = map(moisture, minValue, maxValue, 0, 100);

  Serial.print("Moisture Percentage : ");
  Serial.print(moisturePercent);
  Serial.println("%");
  
  lcd.setCursor(0, 0);
  lcd.print("moisture:");
  lcd.print(moisturePercent);
  lcd.print("%");
}

// code for temperature
void temperature() {
   sensors.requestTemperatures(); 

  //print the temperature in Celsius
  // Serial.print("Temperature: ");
  RawValue = sensors.getTempCByIndex(0);
  float CorrectedValue = (((RawValue - RawLow) * ReferenceRange) / RawRange) + ReferenceLow;
  total = total - readings[readIndex];
  
  // Add the new reading to the total
  total = total + CorrectedValue;
  
  // Store the new reading in the array
  readings[readIndex] = CorrectedValue;
  
  // Increment the index for the next reading
  readIndex = (readIndex + 1) % numReadings;
  
  // Calculate the average
  average = (float)total / numReadings;
}


