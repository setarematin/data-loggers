#include <RTClib.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "DHT.h"

#define DHTPIN 2 
#define DHTTYPE DHT11
#define USE_ARDUINO_INTERRUPTS true   
 
RTC_DS1307 rtc;

const int chipSelect = 4;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  while (!Serial);
  
  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("card initialized.");

  Serial.print("humidity");
  Serial.print(", ");
  Serial.print("tempreture");
  Serial.println("");

  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  File dataFile = SD.open("temp.txt", FILE_WRITE);
  dataFile.println("Date           , Time        , humidity  , temperature");
  dataFile.close();
    Serial.println("Date      , Time     , humidity  , temperature");
}


void loop() {
   delay(1000);
  String hum = "";
  String temp = "";

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  if(h)
  {
    hum = String(h);
  }

  if(t)
  {
    temp = String(t);
  }

  File dataFile = SD.open("temp.txt", FILE_WRITE);
    DateTime now = rtc.now();  
  if (dataFile) 
  {
        Serial.print(now.day());
        Serial.print('-');
        Serial.print(now.month());
        Serial.print('-');
        Serial.print(now.year());
        Serial.print(" , ");
        Serial.print(now.hour());
        Serial.print(':');
        Serial.print(now.minute());
        Serial.print(':');
        Serial.print(now.second());
        Serial.print(" , ");
        Serial.print(hum);
        Serial.print("     , ");
        Serial.println(temp); 

        dataFile.print(now.day());
        dataFile.print('-');
        dataFile.print(now.month());
        dataFile.print('-');
        dataFile.print(now.year());
        dataFile.print(" , ");
        dataFile.print(now.hour());
        dataFile.print(':');
        dataFile.print(now.minute());
        dataFile.print(':');
        dataFile.print(now.second());
        dataFile.print(" , ");          
        dataFile.print(hum);
        dataFile.print("        , ");
        dataFile.println(temp);
        
    dataFile.close();
  }
  else {
    Serial.println("error opening datalog.txt");
    while(1); 
  }

}
