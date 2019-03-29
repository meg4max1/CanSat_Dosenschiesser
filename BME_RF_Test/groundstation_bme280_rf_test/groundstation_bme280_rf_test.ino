#include <SPI.h>
#include <LoRa.h>

#define SX1278_CS   18
#define SX1278_RST  14
#define SX1278_IRQ 26

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

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    Serial.print(",");
    Serial.println(LoRa.packetRssi());

  }
}
