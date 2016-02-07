#include "Arduino.h"
#include "IoExtender.h"
#include "Wire.h"

boolean IoExtender::_wire_init = 0;

IoExtender::IoExtender() {
  _init = 0;  
  _portsStatus = 0;
}

uint8_t IoExtender::init(uint8_t address) {
  _address = address; 
  _init = 1;
  uint8_t result = 0;

  Serial.print("IoExtender(");
  Serial.print(_address);
  Serial.println(")");  

  if (_wire_init == 0) {
    Wire.begin();
    _wire_init = 1;  
  }

  Wire.beginTransmission(_address);
  Wire.write(0x06);  // iodir0 
  Wire.write(0xFF);  // DDR Port0 all input    
  result = Wire.endTransmission(); 

  if (result == 0) {  
    Wire.beginTransmission(_address);
    Wire.write(0x07);  // iodir1 
    Wire.write(0xFF);  // DDR Port1 all input    
    result = Wire.endTransmission();  
  }
  
  return result;
}

void IoExtender::invertPortsPolarity() {
  Wire.beginTransmission(_address);
  Wire.write(0x04);  // ipol0 
  Wire.write(0xFF);  // DDR Port0 invert all ports   
  Wire.endTransmission(); 

  
  Wire.beginTransmission(_address);
  Wire.write(0x05);  // ipol1 
  Wire.write(0xFF);  // DDR Port1 invert all ports   
  Wire.endTransmission();   
}

uint16_t IoExtender::getCurrentPortStatus() {
  uint8_t port0 = 0;
  uint8_t port1 = 0;
  uint16_t result = 0;
  
  Wire.beginTransmission(_address);
  Wire.write(0x00);
  Wire.endTransmission();   

  Wire.requestFrom(_address, (uint8_t)1);
  port0 = Wire.read();

  Wire.beginTransmission(_address);
  Wire.write(0x01);
  Wire.endTransmission();  

  Wire.requestFrom(_address, (uint8_t)1);
  port1 = Wire.read();

  result |= port1;
  result = result << 8;
  result |= port0;

  return result;   
}

uint16_t IoExtender::getPortsStatus() {    
  _portsStatus = getCurrentPortStatus();
  
  return _portsStatus;  
}

void IoExtender::getChanges(int8_t changes[]) {
  uint16_t index = 0;
  uint16_t mask = 0;
  int16_t actual = 0;
  int16_t prev = 0;
  
  uint16_t current = getCurrentPortStatus();  

  for (index=0;index<IO_EXTENDER_PORT_NUMBER;index++) {
    actual = 0;
    prev = 0;
    mask = 1 << index;    

    actual = (current & mask) / mask;
    prev = (_portsStatus & mask) / mask;   

    changes[index] = actual - prev;    
  } 
  
  _portsStatus = current;
  
}


