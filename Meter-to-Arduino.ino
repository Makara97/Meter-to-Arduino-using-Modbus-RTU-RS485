#include <CRC16.h>
#include <SoftwareSerial.h>
#include <string.h>

const int rxPin = 2; //tx pin
const int txPin = 3; //rx pin
SoftwareSerial rs485(rxPin, txPin); // Create RS485 software serial object

const byte slave_address = 0x0A;
const byte function_code = 0x03;
const byte data[] = {0x00, 0x00, 0x00, 0x02}; // Example data (2 bytes)
const byte CRCC[] = {0x00, 0x00};

void setup() {
  Serial.begin(9600);
  // Configure RS485 serial port
  rs485.begin(9600);

  
}

void loop() {
  byte frame[] = {slave_address, function_code, data[0], data[1], data[2], data[3], CRCC[0], CRCC[1]};

  CRC16 crc(0x8005, 0xFFFF, 0x0000, true, true);
  crc.add(frame, sizeof(frame)-2); // Exclude CRC bytes
  uint16_t crcValue = crc.getCRC();
  frame[sizeof(frame) - 2] = crcValue & 0xFF;
  frame[sizeof(frame) - 1] = crcValue >> 8;

  Serial.print("Requrest-- ");
  for (int i = 0; i < sizeof(frame); i++) {
    Serial.print(frame[i], HEX);
    Serial.print(" ");
  }
  Serial.println(" ");

  // Send the msg
  rs485.write(frame, sizeof(frame));

  //wait for thr slave respond
  delay(10);

  //Read the response 
  int dataIndex = 0; //starting point the buffer for response
  byte dataBuf[256]; //buffer for the response msg

  // for (int i=0; i<256; i++){
  //   dataBuf[i] = 0;
  // }

  while(rs485.available()>0){
    //read the byte
    byte data = rs485.read();
    //store the data in array
    dataBuf[dataIndex] = data;
    dataIndex++;
  }
  
  delay(100);

  //print the stored data
  Serial.print("Received-- ");
  for (int i = 0; i < 10; i++) {
    Serial.print(dataBuf[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  dataIndex = 0;

  uint32_t result = result | (dataBuf[3]<<24) |  (dataBuf[4]<<16) |  (dataBuf[5]<<8) |  (dataBuf[6]);

  Serial.print("Voltage: ");
  Serial.println((float)result/10,2);

  Serial.println("\n");
  delay(250);
}

// // CRC calculation function (replace with your preferred CRC implementation)
// word calculateCRC(byte* data, byte length) {
//   word crc = 0xFFFF;
//   for (byte i = 0; i < length; i++) {
//     crc ^= data[i];
//     for (byte j = 0; j < 8; j++) {
//       if (crc & 0x0001) {
//         crc = (crc >> 1) ^ 0xA001;
//       } else {
//         crc >>= 1;
//       }
//     }
//   }
