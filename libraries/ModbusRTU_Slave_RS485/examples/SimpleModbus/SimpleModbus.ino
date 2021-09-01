#include <ModbusRTUSlave.h>

// size of data which will be read and written
#define DATA_SIZE 100
// data array which will be read and written
u16 _D[DATA_SIZE];

// address (kind of name) of above data, may be anything
#define VIRTUAL_ADDRESS 0x7000

#define OUR_ID_AS_A_SLAVE 101
#define PIN_CONNECTED_TO_BOTH_DE_AND_RE 3

ModbusRTUSlave rtu(OUR_ID_AS_A_SLAVE, &Serial1, PIN_CONNECTED_TO_BOTH_DE_AND_RE);

void setup()
{ 
  rtu.addWordArea(VIRTUAL_ADDRESS, _D, DATA_SIZE);
  rtu.begin(9600);

  //Serial.begin(9600); // not needed, for logging purpose only

  // set some value in data array to test if master can read and modify it
  _D[0] = 160;
}

void loop()
{
  // waiting for requests from master
  // reading and writing _D according to requests from master happens here
  rtu.process();
}
