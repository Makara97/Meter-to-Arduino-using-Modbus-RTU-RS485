#include <SoftwareSerial.h>

#define RS_RO    10
#define RS_DI    11
#define RS_DE_RE 12

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
  if(Serial.available()){
    //active the transmitter
    digitalWrite (RS_DE_RE, HIGH);
    //send the data
    RS_Master (Serial.read());
    //active the receivers
    digitalWrite (RS_DE_RE, LOW);
  }
  if(RS_Master.available()){
    Serial.write(RS_Master.read());
  }
}
