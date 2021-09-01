#include <ModbusRTUSlave.h>

u16 _D[100];
u8 _M[50];

ModbusRTUSlave rtu(1, &Serial1);

void setup() {
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  
  rtu.addWordArea(0x7000, _D, 100);
  rtu.addBitArea(0x1000, _M, 50);
  rtu.begin(9600);
}

void loop() {
  rtu.process();
  digitalWrite(3, getBit(_M,0));
  digitalWrite(4, getBit(_M,1));
}