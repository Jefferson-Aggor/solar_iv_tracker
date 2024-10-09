#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

RTC_DS1307 rtc;
static DateTime now;
String time_ = "";
String tfData = "";

float adc_voltage = 0;
float in_voltage = 0;

float R1 = 33000;
float R2 = 10000;

float ref_voltage = 5.0;
int adc_value = 0;
const int chipSelect = 2;

float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;

void setup() {
  
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
  lcd.clear();

  lcd.home();
  lcd.print(F("SOLAR PANEL "));
  lcd.setCursor(0, 1);
  lcd.print("MONITOR");

  Serial.begin(115200);

  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  Serial.println("Measuring voltage and current with INA219 ...");

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  File dataFile = SD.open("DATA.txt", FILE_WRITE);

  Wire.begin();

  //init the rtc
  rtc.begin();

  //rtc.adjust(DateTime(2024, 2, 24, 15, 30, 00));
 
  now = rtc.now();


  delay(3000);
  lcd.clear();
}

void loop() {

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");

  delay(1000);
  
  adc_value = analogRead(A1);
  adc_voltage = (adc_value * ref_voltage)/1024.0;
  in_voltage = adc_voltage*(R1+R2)/R2;

  Serial.println("Input Voltage = ");
  Serial.println(in_voltage, 2);

  delay(1000);

  lcd.home();
  lcd.print("Volt: " + String(in_voltage) + "V ");
  lcd.setCursor(0, 1);
  lcd.print("Curr: " + String(current_mA) + "mA ");

  log_data();

}

void log_data()
{
  //("Date,Time,Temperature,Humidity,CO2,methane,ammonia,H2S,pressure,level")
  File dataFile = SD.open("DATA.txt", FILE_WRITE);
  String _time = getTimeStamp();
  Serial.println(_time);
  if (dataFile) {
    dataFile.print(_time); 
    dataFile.print(","); 
    dataFile.print(String(in_voltage)); 
    dataFile.print(",");
    dataFile.print(String(current_mA));
    dataFile.println(); 
    dataFile.close(); 

  }
  
  else
  Serial.println("OOPS!! SD card writing failed");
}

String getTimeStamp()
{
  now = rtc.now();
  String timeStamp = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + "|" +
  String(now.year()) + "/" + String(now.month()) + "/" + String(now.day());

  return timeStamp;
}
