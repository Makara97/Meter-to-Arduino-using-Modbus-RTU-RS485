#include <SoftwareSerial.h>

#define RS_RO    0
#define RS_DI    1
#define RS_DE_RE 2

String sid;
String mid;
String noblock = " ";

SoftwareSerial RS_Master(RS_RO,RS_DI); //RX,TX

void setup() {
  Serial.begin (9600);
  RS_Master.begin (9600);
  pinMode (RS_DE_RE, OUTPUT);
  //master need to active RS_DE_RE to high to start the transmition
  digitalWrite(RS_DE_RE, HIGH);
  Serial.println("Master node is ready.....");
}

void loop() {
  //Get the slaveID, MemoryID and number of blocks
  Serial.println("Enter the slaveID to request the data");
  if(Serial.available()){
    sid = Serial.readStringUntil('\n');
    Serial.println("Enter the starting memory addressID of the request data");
    if(Serial.available()){
      mid = Serial.readStringUntil('\n');
      
      if(mid.toInt() > 30000){
        Serial.println("Enter the number of blockes in the request data");
        if(Serial.available()){
          String noblock = Serial.readStringUntil('\n');
        }
      }
    }
  }

//create the request msg for the  
Serial.println("Geting the requested data from "+ sid + " " + mid + " "+ noblock);
Serial.println("wait......");
  //msg create here

//request the data using created msg from slaves and wait until response
  //msg post here
RS_Master (Serial.read()); //need to edit the serial.read to msgs
digitalWrite (RS_DE_RE, LOW);
delay(100); //or wait for the response, need to add timeout here

//save the response msg in memory
if(RS_Master.available()){
  string rev = RS_Master.read();
  Serial.println("Here the data from the device");
  Serial.println("slaveid:"+slaveid+" "+"Memoryid:"+memoryid+" "+"Data:"+data);
}
digitalWwite(RS_DE_RE, HIGH);
}
