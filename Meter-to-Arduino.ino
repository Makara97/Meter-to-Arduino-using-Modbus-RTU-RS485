#include <CRC16.h>
#include <SoftwareSerial.h>
#include <string.h>

/*only 2 parameters can be get from the Slave, when we try get more of them,
the results become wrong one
parameter 2 - 2259 0
parameter 3 - 2259 18 0
parameter 3 - 0 85596 55662*/

const int rxPin = 2;
const int txPin = 3;
SoftwareSerial rs485(rxPin, txPin); // Create RS485 software serial object

const byte slave_address = 0x0A;
const byte function_code = 0x03;
const byte data[] = {0x00, 0x00, 0x00, 0x04};
const byte CRCC[] = {0x00, 0x00};

void setup() {
  Serial.begin(9600);
  rs485.begin(9600); // Configure RS485 serial port
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
  byte dataBuf[64] = {0}; //buffer for the response msg

  while(rs485.available()>0){
    //read the byte
    byte dataR = rs485.read();
    //store the data in array
    dataBuf[dataIndex] = dataR;
    dataIndex++;
  }
  

  //print the stored data
  Serial.print("Received-- ");
  for (int i = 0; i < dataIndex; i++) {
    Serial.print(dataBuf[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  int j = 0;
  uint32_t result[j]={0}; 
  
  
  int k = data[3] * 2;
  Serial.println("Received bytes: "+String(k));

  delay(100);

  //int i = 3;
  //result[0] = {(dataBuf[i]<<24) |  (dataBuf[i+1]<<16) |  (dataBuf[i+2]<<8) |  (dataBuf[i+3]) };
  //i = 7;
  //result[1] = {(dataBuf[i]<<24) |  (dataBuf[i+1]<<16) |  (dataBuf[i+2]<<8) |  (dataBuf[i+3]) };
  
  // for(int i=3; i<9; i+4){
  //   result[j] = {(dataBuf[i]<<24) |  (dataBuf[i+1]<<16) |  (dataBuf[i+2]<<8) |  (dataBuf[i+3]) };
  //   j++; 
  // }

  int i = 3;
  while(i< k){
    result[j] = {(dataBuf[i]<<24) |  (dataBuf[i+1]<<16) |  (dataBuf[i+2]<<8) |  (dataBuf[i+3]) };
    j++;
    i+=4;
  }


  Serial.print("combined array-- ");
  for (int i = 0; i < j ; i++) {
    Serial.print(result[i]);
    Serial.print(" ");
  }
  Serial.println();

  delay(1000);
}
/*only 2 parameters can be get from the Slave, when we try get more of them,
the results become wrong one
parameter 2 - 2259 0
parameter 3 - 2259 18 0
parameter 3 - 0 85596 55662*/

