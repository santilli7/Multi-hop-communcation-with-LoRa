#include <SPI.h>
#include <LoRa.h>

int counter = 0;
void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.onReceive(onReceive);
  LoRa.receive();
}

void loop() {
  /*
  // try to parse packet
  //Receiver
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    //Serial.print("Received packet '");
    
    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

     // Sending Ack
      LoRa.beginPacket();
      //LoRa.print("Packet n: ");
      LoRa.print(" ack");
      //LoRa.print(dht.readTemperature());
      LoRa.endPacket();  

    Serial.print("ack");
    // print RSSI of packet
    Serial.print(",");
    Serial.print(LoRa.packetRssi());
    Serial.print(",");
    Serial.print(LoRa.packetSnr());
    Serial.print(",");
    Serial.print(LoRa.packetFrequencyError());
    Serial.println(" ");
  }
  */
}

void onReceive(int packetSize) {
  // received a packet
  Serial.print("Received packet '");

  // read packet
  for (int i = 0; i < packetSize; i++) {
    Serial.print((char)LoRa.read());
  }

  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());
  
}
