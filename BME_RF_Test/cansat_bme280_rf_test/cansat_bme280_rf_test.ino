
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <SPI.h>
#include <LoRa.h>

#define SX1278_CS   18
#define SX1278_RST  14
#define SX1278_IRQ 26

#define BME_ADDR 0x76 

Adafruit_BME280 bme;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const long interval = 500;

String sensorVals;

void setup() {
 pinMode(LED_BUILTIN,OUTPUT);
 digitalWrite(LED_BUILTIN,LOW);
 LoRa.setPins(SX1278_CS, SX1278_RST, SX1278_IRQ);
 while (!LoRa.begin(868E6)) {
  blinkled(200);
 }
 
 while(!bme.begin(BME_ADDR)){
  blinkled(500);
 }
 while(!SD.begin()){
  blinkled(1000);
 }

 
}

void loop() {
  currentMillis = millis();
  if (millis() - previousMillis >= interval) {
    sensorVals = readSensors();
    LoRa.beginPacket();
    LoRa.print(sensorVals);
    LoRa.endPacket();
    previousMillis = currentMillis;
  }
}

String readSensors(){
  int temperature = bme.readTemperature()*100;
  int humidity = bme.readHumidity()*100;
  int pressure = bme.readPressure()*100;
  
  String assembleString = "{";
  assembleString = assembleString + temperature + "," + humidity + "," + pressure ;
  assembleString +=  "}";
  return assembleString;
}

void blinkled(int frequency){
  pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(frequency);
    digitalWrite(LED_BUILTIN, LOW);
    delay(frequency);
}
