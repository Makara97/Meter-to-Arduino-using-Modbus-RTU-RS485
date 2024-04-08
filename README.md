# Meter to Arduino
 This code is used to get the data from the CVM NRG96 energy meter manufactured by CIRCUTOR Ltd, to Arduino UNO development board. Here, we are using the MODBUS RS485 protocol for comminication between the devices.  

### Introduction

This code is developed to communicate between Energy meters which has the RS485 connectivity and the Arduino development board. 

### Pre-requested items and software

Before we stablish the communicaton, we have to collect following devices and software.
* Energy meter that has the Modbus RTU RS485 connectivity (Ex: Circutor CVM NRG96) 
* RS485 to TTL module
* Arduino Development Board (Ex: Arduino UNO, Ardunino Mega)
* Arduino IDE Software
    * CRC16 library
    * Software Serial library

### Modbus Protocol

Modbus is a serial communication protocol widely used in industrial automation to enable devices to talk to each other. Here's a breakdown of its key aspects:

* Client-Server (Master-Slave): It follows a client-server model, also referred to as master-slave. In this model, one device acts as the master (usually a Human Machine Interface (HMI) or Supervisory Control and Data Acquisition (SCADA) system) and initiates communication, while other devices (slaves, like sensors or PLCs) respond to the master's requests.

* Open and Royalty-Free:  One of its biggest strengths is that it's an open standard. The specifications are publicly available, and there are no licensing fees for using it. This openness has fostered widespread adoption across different manufacturers.

* Data Communication: Modbus focuses on transferring data between devices. This data can represent various process parameters like sensor readings, control settings, and status information.

* Function Codes: The protocol defines a set of function codes that instruct slave devices on what action to take. These codes specify tasks like reading or writing data from specific memory locations within the slave device.

* Message Structure:  Modbus messages have a specific structure, including device addresses, function codes, data to be transferred, and error checking mechanisms to ensure data integrity.

#### Variations in Modbus Protocol

In this protocol, there are 3 different modes are available. 

* Modbus RTU (Remote Terminal Unit): This is a binary format used over serial connections like RS232 or, more commonly, RS485

* Modbus ASCII: A human-readable format using ASCII characters, also used on serial connections

* Modbus TCP/IP: An adaptation for transmitting Modbus messages over Ethernet networks

For this code, we follow the __Modbus RTU RS485 protocol__.  

### Modbus RTU RS485 Protocol

The Modbus RTU RS485 protocol stands as a cornerstone in industrial communication, renowned for its robustness and simplicity. Developed by Modicon in 1979, Modbus initially served as a communication protocol for connecting programmable logic controllers (PLCs) in industrial environments. Over the years, it has evolved into a widely adopted standard due to its ease of implementation and reliability. 

#### Use Cases:

Modbus RTU RS485 finds extensive application across diverse industries, including manufacturing, energy, and building automation. Some common use cases include:

1.Industrial Automation: Modbus RTU RS485 facilitates communication between PLCs, sensors, actuators, and other industrial devices, enabling seamless control and monitoring of manufacturing processes.

2.Energy Management: In energy management systems, Modbus RTU RS485 enables communication between meters, sensors, and control systems, facilitating efficient energy monitoring and optimization.

3.Building Automation: Building management systems utilize Modbus RTU RS485 to integrate various components such as HVAC systems, lighting controls, and security devices, enabling centralized monitoring and control of building operations.

4.Remote Monitoring: Modbus RTU RS485 is often employed for remote monitoring applications, allowing operators to gather data from distributed sensors and equipment located in remote or hazardous environments.

5.Process Control: In process control applications, Modbus RTU RS485 enables real-time communication between control systems and field devices, ensuring accurate control and monitoring of industrial processes.

#### Memory modules in Modbus Protocol

In this protocol, there are 4 different memory types are used; 2 types are read only and other 2 are read and write capabitities. Also there is another method to divide. 2 of memory them are single bit memory space and used with coils and relays. The other 2 used to work with registers with 32 bit lenth. 

| Register Number | Name | Type |
| ---- | ----- | ---- | 
| 00001-09999 | Discrete Output Coils | read-write |
| 10001-19999 | Discrete Input Contacts | read only |
| 30001-39999 | Analog Input Registers | read only |
| 40001-49999 | Analog Output Holding Registers | read-write |


#### Message structure

Mainly, there are 4 sections available in the Modbus RTU message structure.

| SlaveID | Function code | Special data | CRC |
| ---- | ----- | ----- | -----|

##### SlaveID

The SlaveID is the address of the device (Ex: Energy Meter). It can be from 0 to 247 and 248 to 255 are reserved.

##### Function Code

| Function code | Description | Access type |
| ----- | ---- | ---- | 
| 01 (0x01) | Read Coil status | Read | 
| 02 (0x02) | Read Input status | Read |
| 03 (0x03) | Read Holding Resisters | Read |
| 04 (0x04) | Read Input Registers | Read |
| 05 (0x05) | Force Single Coil | Write |
| 06 (0x06) | Preset Single Register | Write |
| 15 (0x0F) | Force Multiple Coils | Write |
| 16 (0x10) | Preset Multiple Registers | Write |

##### CRC

To calculate the CRC value, we have to setup the parameter according to the this, 

Polynommial - 0x8005
Initial mask - 0xFFFF
After mask - 0x0000
Before shift - True
After shift - True

#### Data request and data recieved message 

This is an example that request data from a meter and its answer.

Request - 11 03 006B 0003 7687
Recieve - 11 03 06 AE41 5652 4340 49AD

| Byte (HEX) | Request | Byte (HEX) | Recieve |
| ---- | ---- | ----- | ---- |
| 11 | Device address | 11 | Device address |
| 03 | Function address | 03 | Function address |
| 00 | Address of the first register Hi byte | 06 | Number of bytes more |
| 6B | Address of the first register Lo byte | AE | Register value Hi #30108 |
| 00 | Number of registers Hi bytes | 41 | Register value Lo #30108 |
| 03 | Number of registers Lo bytes | 56 | Register value Hi #30109 |
| 76 | Checksum CRC | 52 | Register value Lo #30109 |
| 87 | Checksum CRC | 43 | Register value Hi #30110 |
| | | 40 | Register value Lo #30110 |
| | | 49 | Checksum CRC |
| | | AD | Checksum CRC |

### How to get the relevant data from the meter

#### Select the RX and TX pins

To stablish the serial communication, we used the Software Serial instand of the Hardware Serial due the advantages of the Software serial method.
In this code, we used the pin number 2 as the RX pin and the pin number 3 as the TX pin.

#### Connection

RS485 to TTL module need to connect with the arduino board according to the following,
both RX pins and TX pins need to be interconnected. (Ex: Module RX pin to Arduino RX pin)

#### Data request

```
const uint8_t slave_address = 0x03;
const uint8_t function_code = 0x03; 
const uint8_t data[] = {0x00, 0x54, 0x00, 0x0C}; 
const uint8_t CRCC[] = {0x00, 0x00}; 
```
```
uint8_t frame[] = {slave_address, function_code, data[0], data[1], data[2], data[3], CRCC[0], CRCC[1]};
```

The slave_address is the meter ID that is working with.

The function_code is already describe in the above.

The data request need to be set as above. In data[] array, the first 2 places are the starting register number, the next places are the number of data words that need to request from the meter. (1 parameter = 2 words = 4 bytes)

The calculated CRC values are replaced the CRCC[] array.

#### CRC calculation

```
CRC16 crc(0x8005, 0xFFFF, 0x0000, true, true); //polynomial, initial mask, after mask, before shift, after shift 
crc.add(frame, sizeof(frame)-2); // Exclude CRC bytes
uint16_t crcValue = crc.getCRC();
frame[sizeof(frame) - 2] = crcValue & 0xFF; //due to the MODBUS msg protocol, have to shift the registers of CRC
frame[sizeof(frame) - 1] = crcValue >> 8;
```
The polynomial data that need to calculate the CRC, need to be set as above for the Modbus RTU. Then, the calculated CRC is placed in the frame array. 

#### Send the frame to slave device

To send the data request to the device, need to send the frame to it using the following way,

` rs485.write(frame, sizeof(frame)); `

#### Capture the response from the device

In the Atmega MCUs (used in Arduino), the serial recieving buffer is only 64 bytes. Therefore, all the recieving data need to be 64 bytes or less. Otherwise, the data more than 64 bytes overflows and cannot recover. 

```
uint8_t dataIndex = 0; //starting point the buffer for response
uint8_t dataBuf[64] = {0}; //buffer for the response msg

while(rs485.available()>0){
    uint8_t dataR = rs485.read();  //read the byte
    dataBuf[dataIndex] = dataR; //store the data in array
    dataIndex++;
}
```

Then the recieved data need to be group according to the Modbus RTU protocol. 

#### Group the bytes to get the results

As prevously mentions, each parameter comes with 4 bytes and Modbus protocol follows the big-endian structure (Heigher values come first). 

