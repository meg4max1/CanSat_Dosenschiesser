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

void setup() {
 pinMode(LED_BUILTIN,OUTPUT);
 digitalWrite(LED_BUILTIN,LOW);
 LoRa.setPins(SX1278_CS, SX1278_RST, SX1278_IRQ);
 if (!LoRa.begin(866E6)) {
  digitalWrite(LED_BUILTIN,HIGH);
 }
 
 if(!bme.begin(BME_ADDR)){
  digitalWrite(LED_BUILTIN,HIGH);
 }

 
}

void loop() {
  LoRa.beginPacket();
  LoRa.print(bme.readTemperature());
  LoRa.print(",");
  LoRa.print(bme.readHumidity());
  LoRa.print(",");
  LoRa.print(bme.readPressure());
  LoRa.endPacket();

}
