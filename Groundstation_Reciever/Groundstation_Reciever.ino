#include <TinyGPS++.h> 
#include <HardwareSerial.h> 

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADXL345_U.h>
#include <QMC5883.h>

#include <SPI.h>
#include <LoRa.h>

#define SX1278_CS   18
#define SX1278_RST  14
#define SX1278_IRQ 26

#define BME_ADDR 0x76 

HardwareSerial gpsSerial(1); 
TinyGPSPlus gps; 

Adafruit_BME280 bme;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(11111);
QMC5883 qq;

String sendSerial = " ";

void setup() {
 pinMode(LED_BUILTIN,OUTPUT);
 digitalWrite(LED_BUILTIN,LOW);
 gpsSerial.begin(9600, SERIAL_8N1, 16, 17); 
 LoRa.setPins(SX1278_CS, SX1278_RST, SX1278_IRQ);
 LoRa.setSyncWord(0x42);
 if (!LoRa.begin(863E6)) {
  digitalWrite(LED_BUILTIN,HIGH);
 }
 bme.begin(BME_ADDR);
 accel.begin();
 qq.begin();
 Serial.begin(9600);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    Serial.print(",");
    Serial.print(LoRa.packetRssi());
    Serial.println(readSensors());
  }
  while (gpsSerial.available()) gps.encode(gpsSerial.read()); 
 /*currentMillis = millis();
  if (millis() - previousMillis >= interval) {

    
    previousMillis = currentMillis;
  }*/
}

String readSensors(){
  sensors_event_t event;
  int temperature = bme.readTemperature()*10;
  int humidity = bme.readHumidity()*10;
  int pressure = bme.readPressure()*10;
  accel.getEvent(&event);
  qq.calculate();
  int elevationangle = atan2(event.acceleration.y, event.acceleration.z)/(0.001745329);
  int azimuthangle = qq.getHeadingDegree('z');
  int sat = gps.satellites.isValid() ? gps.satellites.value() : 0; 
  int latitude = gps.location.isValid() ? gps.location.lat()*100000 : 0; 
  int longtitude = gps.location.isValid() ? gps.location.lng()*100000 : 0;
  int height = gps.location.isValid() ? gps.altitude.meters()*10 : 0;
  String assembleString = "," ;
  assembleString = assembleString + 
  temperature + "," + 
  humidity + "," + 
  pressure + "," + 
  elevationangle + "," +
  azimuthangle + "," +
  sat + "," + 
  latitude + "," + 
  longtitude + "," +
  height;
  return assembleString;
}


void blinkled(int frequency){
  pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(frequency);
    digitalWrite(LED_BUILTIN, LOW);
    delay(frequency);
}
