#include <CRC16.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <stdint.h>

const uint8_t rxPin = 2;
const uint8_t txPin = 3;
SoftwareSerial rs485(rxPin, txPin); // Create RS485 software serial object

const uint8_t slave_address = 0x0A;
const uint8_t function_code = 0x03;
const uint8_t data[] = {0x00, 0x00, 0x00, 0x02};
const uint8_t CRCC[] = {0x00, 0x00};

void setup() {
  Serial.begin(19200);
  rs485.begin(19200); // Configure RS485 serial port
}

void loop() {
  uint8_t frame[] = {slave_address, function_code, data[0], data[1], data[2], data[3], CRCC[0], CRCC[1]};

  CRC16 crc(0x8005, 0xFFFF, 0x0000, true, true);
  crc.add(frame, sizeof(frame)-2); // Exclude CRC bytes
  uint16_t crcValue = crc.getCRC();
  frame[sizeof(frame) - 2] = crcValue & 0xFF;
  frame[sizeof(frame) - 1] = crcValue >> 8;

  Serial.print("Requrest-- ");
  for (uint8_t i = 0; i < sizeof(frame); i++) {
    Serial.print(frame[i], HEX);
    Serial.print(" ");
  }
  Serial.println(" ");

  // Send the msg
  rs485.write(frame, sizeof(frame));

  //wait for thr slave respond
  delay(10);

  //Read the response 
  uint8_t dataIndex = 0; //starting point the buffer for response
  uint8_t dataBuf[64] = {0}; //buffer for the response msg

  while(rs485.available()>0){
    //read the byte
    uint8_t dataR = rs485.read();
    //store the data in array
    dataBuf[dataIndex] = dataR;
    dataIndex++;
  }
  
  //print the stored data
  Serial.print("Received-- ");
  for (uint8_t i = 0; i < dataIndex; i++) {
    Serial.print(dataBuf[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  uint8_t i = 0;
  uint8_t k = dataBuf[2] / 4;
  uint32_t result[k] = {0};
  
  Serial.println(" variables k "+ String(k));

  Serial.println("dataBuf "+ String(dataIndex)) ;
  uint8_t z[dataBuf[2]];
  uint8_t q = 3;
  uint8_t b = 0;
  while (q < (dataIndex-2)){
    z[b] = dataBuf[q]; 
    Serial.print(String(dataBuf[q],HEX));
    b++;
    q++;
  }
  Serial.println();

  Serial.println("# of registers z "+ String(sizeof(z)));
  
  Serial.print("combined array before-- ");
  for (uint8_t p = 0; p < sizeof(z) ; p++) {
    Serial.print(z[p],HEX);
    Serial.print(" ");
  }
  Serial.println();

  //  while (i < k ) {
  // // Loop for each byte within the element
  //   for (uint8_t j = 0; j < 4; j++) {
  //     // Shift and combine bytes, handling potential index overflow
  //     uint8_t index = i * 4 + j;
  //     result[i] |= z[index] << (24 - 8 * j);
  //   }
  //   i++;
  // } 
 

  //result[0] = z[0] * 16777216 + z[1] * 65536 + z[2] * 256 + z[3];
  //result[1] = z[4] * 16777216 + z[5] * 65536 + z[6] * 256 + z[7];

  uint8_t j = 0; 
  while (i < k ) {
  // Loop for each byte within the element
    result[i] = z[j] * 16777216 + z[j+1] * 65536 + z[j+2] * 256 + z[j+3];
    j+=4;
    i++;
  }
  
  // Serial.println(result[0]);
  // Serial.println(result[1]);

  for (uint8_t p = 0; p < i ; p++) {
    Serial.print(result[p]);
    Serial.print(" ");
  }
  Serial.println();
  

  // Serial.print("combined array after-- ");
  // for (uint8_t p = 0; p < i ; p++) {
  //   Serial.print(result[p]);
  //   Serial.print(" ");
  // }
  // Serial.println();

  delay(1000);
}



