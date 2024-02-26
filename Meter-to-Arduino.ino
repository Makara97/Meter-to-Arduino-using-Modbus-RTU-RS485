#include <CRC.h>
#include <SoftwareSerial.h> 

const int txPin = 1;
const int rxPin = 0;
SoftwareSerial rs485(txPin, rxPin); // Create RS485 software serial object

const byte slave_address = 1;
const byte function_code = 3;
const byte data[] = {0, 0, 0, 2}; // Example data (2 bytes)
const byte CRCC[] = {0, 0};

void setup() {
   // Begin serial communication for debugging
  Serial.begin(9600);
  // Configure RS485 serial port
  rs485.begin(9600);
  pinMode(txPin, OUTPUT);
  pinMode(rxPin, INPUT);
}

void loop() {
  byte frame[] = {slave_address, function_code, data[0], data[1], data[2], data[3], CRCC[1], CRCC[2]};

  CRC16 crc(0x8005, 0xFFFF, 0x0000, true); // CRC-16-IBM with bit-reversing
  // Calculate CRC
  crc.add(frame, sizeof(frame)); // Exclude CRC bytes
  uint16_t crcValue = crc.getCRC();

  // Append CRC to frame (reverse byte order)
  frame[sizeof(frame) - 2] = crcValue & 0xFF;
  frame[sizeof(frame) - 1] = crcValue >> 8;

  //send Modbus frame over RS485
  rs485.write(frame, sizeof(frame)); 

  Serial.print("Request: ");
  //for debug the frame
  for (int i = 0; i < sizeof(frame); i++) {
    Serial.print(frame[i], HEX);
    Serial.print(" ");
  }
  Serial.println(" ");

  Serial.println("Waiting for the response.......");
  delay(100);

  while (!rs485.available()) {  // Wait for data
    delay(100);  // Small delay to avoid flooding loop
  }

  Serial.print ("Response: ");
   res = rs485.read();

  Serial.print(res);
  Serial.println(" ");
  delay(1000);
}
