#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//#include <Adafruit_ADXL345_U.h>

#include <SPI.h>
#include <LoRa.h>

#define SX1278_CS   18
#define SX1278_RST  14
#define SX1278_IRQ 26

#define BME_ADDR 0x76 

Adafruit_BME280 bme;
//Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

String sendSerial;

void setup() {
 pinMode(LED_BUILTIN,OUTPUT);
 digitalWrite(LED_BUILTIN,LOW);
 LoRa.setPins(SX1278_CS, SX1278_RST, SX1278_IRQ);
 if (!LoRa.begin(868E6)) {
  digitalWrite(LED_BUILTIN,HIGH);
 }
 Serial.begin(9600);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    while (LoRa.available()) {
      sendSerial += (char)LoRa.read();
    }
    sendSerial += readSensors();
    Serial.print(",");
    Serial.println(LoRa.packetRssi());
    Serial.print("}");
  }
}

String readSensors(){
  int temperature = bme.readTemperature()*10;
  int humidity = bme.readHumidity()*10;
  int pressure = bme.readPressure()*10;
  //sensors_event_t event; 
  //accel.getEvent(&event);
  //int acceleration = (1000 * sqrt(sq(event.acceleration.x) + sq(event.acceleration.y) + sq(event.acceleration.z)));
  String assembleString = ",";
  assembleString = assembleString + temperature + "," + humidity + "," + pressure ;//+ "," + acceleration ;
  return assembleString;
}

void blinkled(int frequency){
  pinMode(LED_BUILTIN,OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(frequency);
    digitalWrite(LED_BUILTIN, LOW);
    delay(frequency);
}
