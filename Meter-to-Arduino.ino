/*
This code is the designed to get the data from the CIRCUTOR CVM-NRG96 energy meter and store at the Arduino UNO board.
Therefore, with this code, Arduino UNO board can work as a master. 

  Name of the Engineer : Kavindu Makaranda Mohottala
  Data : 13 / 02 / 2024
  Version : v4.1.1

*/

#include <CRC16.h>
#include <SoftwareSerial.h>
#include <stdint.h>

const uint8_t rxPin = 2; // these pin number naming are same as the module pin naming
const uint8_t txPin = 3;
SoftwareSerial rs485(rxPin, txPin); // Create RS485 software serial object

const uint8_t slave_address = 0x03; //slave device address
const uint8_t function_code = 0x03; 
const uint8_t data[] = {0x00, 0x8C, 0x00, 0x1C}; // starting register number and number of requesting registers
const uint8_t CRCC[] = {0x00, 0x00}; //for the CRC values

void setup() {
  Serial.begin(9600);
  rs485.begin(9600); // Configure RS485 serial port
}

void loop() {
  uint8_t frame[] = {slave_address, function_code, data[0], data[1], data[2], data[3], CRCC[0], CRCC[1]};

  CRC16 crc(0x8005, 0xFFFF, 0x0000, true, true); //polynomial, initial mask, after mask, before shift, after shift 
  crc.add(frame, sizeof(frame)-2); // Exclude CRC bytes
  uint16_t crcValue = crc.getCRC();
  frame[sizeof(frame) - 2] = crcValue & 0xFF; //due to the MODBUS msg protocol, have to shift the registers of CRC
  frame[sizeof(frame) - 1] = crcValue >> 8;

  Serial.print("Requrest-- ");
  for (uint8_t i = 0; i < sizeof(frame); i++) {
    Serial.print(frame[i], HEX);
    Serial.print(" ");
  }
  Serial.println(" ");

  rs485.write(frame, sizeof(frame)); // Send the MODBUS frame to the Slave device

  delay(10); //wait for thr slave respond

  //Read the response 
  uint8_t dataIndex = 0; //starting point the buffer for response
  uint8_t dataBuf[64] = {0}; //buffer for the response msg

  while(rs485.available()>0){
    uint8_t dataR = rs485.read();  //read the byte
    dataBuf[dataIndex] = dataR; //store the data in array
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
  uint8_t k = dataBuf[2] / 4; // number of parameters due to the 2 words = 1 parameter
  uint32_t result[k] = {0}; 
  
  uint8_t j = 3; 
  while (i < k ) {
    result[i] = dataBuf[j] * 16777216 + dataBuf[j+1] * 65536 + dataBuf[j+2] * 256 + dataBuf[j+3];
    // 1 parameter = 0x00, 0x00, 0x08, 0xF9 = 2297
    j+=4;
    i++;
  }

  // for debugging purposes
  // for (uint8_t p = 0; p < i ; p++) {
  //   Serial.print(result[p]);
  //   Serial.print(" ");
  // }
  // Serial.println();
/*
  Serial.println("For line 1: ");
  Serial.print("Voltage (v): ");
  Serial.println((float)result[0]/10,2);
  Serial.print("Voltage (mA): ");
  Serial.println(result[1]);
  Serial.print("ACtive Power (W): ");
  Serial.println(result[2]);
  Serial.print("Reactive Power (VAR): ");
  Serial.println(result[3]);
  Serial.print("Power Fctor (x100): ");
  Serial.println(result[4]);

  Serial.println();
  Serial.println("For line 2: ");
  Serial.print("Voltage (v): ");
  Serial.println((float)result[5]/10,2);
  Serial.print("Voltage (mA): ");
  Serial.println(result[6]);
  Serial.print("ACtive Power (W): ");
  Serial.println(result[7]);
  Serial.print("Reactive Power (VAR): ");
  Serial.println(result[8]);
  Serial.print("Power Fctor (x100): ");
  Serial.println(result[9]);
  
  Serial.println();
  Serial.println("For line 3: ");
  Serial.print("Voltage (v): ");
  Serial.println((float)result[10]/10,2);
  Serial.print("Voltage (mA): ");
  Serial.println(result[11]);
  Serial.print("ACtive Power (W): ");
  Serial.println(result[12]);
  Serial.print("Reactive Power (VAR): ");
  Serial.println(result[13]);
  // Serial.print("Power Fctor (x100): ");
  // Serial.println(result[14]);
*/
  Serial.println();
  delay(1000);
}



