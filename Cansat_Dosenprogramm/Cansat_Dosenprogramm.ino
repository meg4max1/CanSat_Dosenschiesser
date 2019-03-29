

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADXL345_U.h>

#include <SPI.h>
#include <LoRa.h>

#define SX1278_CS   18
#define SX1278_RST  14
#define SX1278_IRQ 26

#define BME_ADDR 0x76 

Adafruit_BME280 bme;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const long interval = 500;

String sensorVals;

void setup() {
 pinMode(LED_BUILTIN,OUTPUT);
 digitalWrite(LED_BUILTIN,LOW);
 LoRa.setPins(SX1278_CS, SX1278_RST, SX1278_IRQ);
 while (!LoRa.begin(868E6)) {
  blinkled(100);
 }
 
 while(!bme.begin(BME_ADDR)){
  blinkled(250);
  blinkled(100);
 }

 while(!accel.begin()){
  blinkled(50);
  blinkled(500);
 }
 accel.setRange(ADXL345_RANGE_8_G);
 accel.setDataRate(ADXL345_DATARATE_12_5_HZ);
/* while(!SD.begin()){
  blinkled(1000);
 }
*/
 
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
  int temperature = bme.readTemperature()*10;
  int humidity = bme.readHumidity()*10;
  int pressure = bme.readPressure()*10;
  sensors_event_t event; 
  accel.getEvent(&event);
  int acceleration = (1000 * sqrt(sq(event.acceleration.x) + sq(event.acceleration.y) + sq(event.acceleration.z)));
  String assembleString = String(temperature) ;
  assembleString = assembleString + "," + humidity + "," + pressure + "," + acceleration ;
  return assembleString;
}

void blinkled(int frequency){
  pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(frequency);
    digitalWrite(LED_BUILTIN, LOW);
    delay(frequency);
}
