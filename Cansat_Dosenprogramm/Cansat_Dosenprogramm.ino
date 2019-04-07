#include <TinyGPS++.h>
#include <HardwareSerial.h>

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

HardwareSerial gpsSerial(1);
TinyGPSPlus gps;

Adafruit_BME280 bme;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const long interval = 250;

String sensorVals;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  
  LoRa.setPins(SX1278_CS, SX1278_RST, SX1278_IRQ);
  LoRa.setSyncWord(0x42);
  LoRa.enableCrc();
  while (!LoRa.begin(867E6)) {
    blinkled(100);
  }

  while (!bme.begin(BME_ADDR)) {
    blinkled(250);
    blinkled(100);
  }

  while (!accel.begin()) {
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
  while (gpsSerial.available()) gps.encode(gpsSerial.read());
}

String readSensors() {
  int temperature = bme.readTemperature() * 10;
  int humidity = bme.readHumidity() * 10;
  int pressure = bme.readPressure() * 10;
  sensors_event_t event;
  accel.getEvent(&event);
  int acceleration = (1000 * sqrt(sq(event.acceleration.x) + sq(event.acceleration.y) + sq(event.acceleration.z)));
  int sat = gps.satellites.isValid() ? gps.satellites.value() : 0;
  int latitude = gps.location.isValid() ? gps.location.lat() * 100000 : 0;
  int longtitude = gps.location.isValid() ? gps.location.lng() * 100000 : 0;
  int height = gps.location.isValid() ? gps.altitude.meters() * 10 : 0;
  String assembleString = " ";
  assembleString =
    assembleString +
    temperature + "," +
    humidity + "," +
    pressure + "," +
    acceleration + "," +
    sat + "," +
    latitude + "," +
    longtitude + "," +
    height;
  return assembleString;
}

void blinkled(int frequency) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(frequency);
  digitalWrite(LED_BUILTIN, LOW);
  delay(frequency);
}
