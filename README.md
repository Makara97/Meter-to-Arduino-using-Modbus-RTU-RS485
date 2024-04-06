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

#### Message structure

Mainly, there are 4 sections available in the Modbus RTU message structure.

| SlaveID | Function code | Special data | CRC |
| ---- | ----- | ----- | -----|



### How to get the relevant data from the meter

