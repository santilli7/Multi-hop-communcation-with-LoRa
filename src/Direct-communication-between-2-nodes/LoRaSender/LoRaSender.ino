#include <SPI.h>
#include <LoRa.h>

int counter = 0;
String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xFF;     // address of this device
byte destination = 0xBB;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends

void setup() {
  Serial.begin(9600);
  //Serial.println("CLEARDATA"); //clears up any data left from previous projects

  //Serial.println("LABEL,Time,  Message ,Time of ack, Ack"); //always write LABEL, so excel knows the next things will be the names of the columns (instead of Acolumn you could write Time for instance)

  //Serial.println("RESETTIMER"); //resets timer to 0
  while (!Serial);
  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

}

void loop() {  /*
  //Sender
  // Sending Packets
  LoRa.beginPacket();
  LoRa.print(counter);
  LoRa.endPacket();
  Serial.print("DATA");

  //Receive ack
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    
    Serial.print("TIME");    
    // received a packet
    //Serial.print("Received packet '");
    Serial.print(","); 
    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());  
    }
  
   }

  
  Serial.print(",");
  Serial.print(counter);
  Serial.print(",TIME");
  Serial.println();

  counter++;
  delay(10000);
 */
  //if (millis() - lastSendTime > interval) {
    //String message = "HeLoRa World!";   // send a message
    
   // Serial.println("Sending " + message);
    //lastSendTime = millis();            // timestamp the message
    //interval = random(2000) + 1000;    // 2-3 seconds
  //}
  onReceive(LoRa.parsePacket());
}

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  Serial.println("Sending Ack to: 0x" + String(destination,HEX));
  msgCount++;                           // increment message ID
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";
  
  // received a packet
   while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }

  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();

  sendMessage("ack");
  Serial.println("Sending Ack to: ");
}
