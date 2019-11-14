/*
  Conf. 1)                      | Conf 2)
  Signal bandwidth --> 125 MhZ  | Signal bandwidth --> 125 MhZ 
  Trasmission power --> 17 db   | Trasmission power --> 15 db
  Radio frequency --> 433 MhZ   | Radio frequency --> 433 MhZ
  Spreading Factor --> 7        | Spreading Factor --> 7
  Coding Rate --> 4/5           | Coding Rate --> 4/5
  Preamble Length --> 8         | Preamble Length --> 16 
*/
#include <SPI.h>              
#include <LoRa.h>

#ifdef ARDUINO_SAMD_MKRWAN1300
#error "This example is not compatible with the Arduino MKR WAN 1300 board!"
#endif

// Configuration 3
//int txPower = 19;
int preambleLength = 16; 


/*Configuration 2
int txPower = 15;
int preambleLength = 16; 
*/

String outgoing;              // outgoing message
byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xAA;     // address of this device
byte destination = 0xFF;      // destination to send to
long lastSendTime = 0;        // last send time
int interval = 2000;          // interval between sends

void setup() {
  Serial.begin(9600);         // initialize serial
  Serial.println("CLEARDATA"); //clears up any data left from previous projects

  Serial.println("LABEL,Time,Millis,Message,RSSI,Frequency Error"); //always write LABEL, so excel knows the next things will be the names of the columns (instead of Acolumn you could write Time for instance)

  Serial.println("RESETTIMER"); //resets timer to 0// initialize serial
  
  while (!Serial);
  
  
  
  if (!LoRa.begin(433E6)) {             // initialize ratio at 433 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  //LoRa.setTxPower(txPower);
  LoRa.setPreambleLength(preambleLength);
  LoRa.onReceive(onReceive);
  LoRa.receive();
}

void loop() {
  
    //lastSendTime = millis();            // timestamp the message
    LoRa.receive();                     
    delay(2000);
}

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID    
  
  Serial.println("Messaggio inviato");
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length
  int msgRSSI = LoRa.packetRssi();      // incoming msg RSSI
  long frequencyErrorMsg = LoRa.packetFrequencyError(); // incoming msg Frequency Error

  String incoming = "";                 // payload of packet

  while (LoRa.available()) {            // can't use readString() in callback, so
    incoming += (char)LoRa.read();      // add bytes one by one
  }

  if (incomingLength != incoming.length()) {   // check length for error
    //Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xAA) {
    //Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }
  sendMessage("ack");
  
  Serial.print((String) "DATA,TIME,");
  Serial.print(millis());
  Serial.print(",");
  Serial.print(incoming);
  Serial.print(",");
  Serial.print(msgRSSI);
  Serial.print(",");
  Serial.print(frequencyErrorMsg);
  Serial.println();
  
}
