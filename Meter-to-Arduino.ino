#include <ArduinoModbus.h>
#include <ArduinoRS485.h>

void setup() {
  Serial.begin(9600);

  // start the Modbus RTU client
  while (!ModbusRTUClient.begin(9600)) {
    Serial.println("Failed to start Modbus RTU Client!");
    delay(200);
  }
}

void loop() {
  
  // send a Holding registers read request to (slave) id 1, for 2 registers
  if (!ModbusRTUClient.requestFrom(1, HOLDING_REGISTERS, 0x00, 2)) {
    Serial.print("failed to read registers! ");
    Serial.println(ModbusRTUClient.lastError());
  } else {
    // If the request succeeds, the sensor sends the readings, that are
    // stored in the holding registers. The read() method can be used to
    // get the raw temperature and the humidity values.
    uint16_t rawtemperature = ModbusRTUClient.read();
    uint16_t rawhumidity = ModbusRTUClient.read();
    uint16_t rawte = ModbusRTUClient.read();
    uint16_t rawhu = ModbusRTUClient.read();

    // To get the temperature in Celsius and the humidity reading as
    // a percentage, divide the raw value by 10.0.
    float temperature = rawtemperature / 10.0;
    float humidity = rawhumidity / 10.0;
    float tem = rawte / 10.0;
    float hum = rawhu / 10.0;
    Serial.println(String(tem)+String(" ")+String(hum));
  }

  delay(500);
}
