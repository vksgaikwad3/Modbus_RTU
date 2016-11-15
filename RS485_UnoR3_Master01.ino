#include <SimpleModbusMaster.h>

#define baud 9600
#define timeout 1000
#define polling 200
#define retry_count 20

#define TxEnablePin 2 

#define LED 13

#define TOTAL_NO_OF_REGISTERS 20        //No of resister in Energy meter

enum
{
  PACKET1,
  PACKET2,
  PACKET3,
  PACKET4,
  PACKET5,
  PACKET6,
  PACKET7,
  PACKET8,
  TOTAL_NO_OF_PACKETS // leave this last entry
};

Packet packets[TOTAL_NO_OF_PACKETS];

unsigned int regs[TOTAL_NO_OF_REGISTERS];

void setup()
{
  Serial.begin(9600);
  //READ_INPUT_REGISTERS
  //READ_HOLDING_REGISTERS
  modbus_construct(&packets[PACKET1], 2,READ_INPUT_REGISTERS , 0, 2, 0);    //30001-2 Line Voltage reg[0-1]       HB :17278 LB :44628
  modbus_construct(&packets[PACKET2], 2,READ_INPUT_REGISTERS , 6, 2, 2);    // 30007-8 Line Current reg[2-3]      
  modbus_construct(&packets[PACKET3], 2,READ_INPUT_REGISTERS , 42,2, 4);    // 30043-44 Device voltage reg[4-5]
  modbus_construct(&packets[PACKET4], 2,READ_INPUT_REGISTERS , 46,2, 6);    // 30047-48 Device Current reg[6-7]
  modbus_construct(&packets[PACKET5], 2,READ_INPUT_REGISTERS , 72,4, 8);    // 30073-74 Power Consumption  reg[8-9]   Reading 16384(73)[IMP]

  modbus_construct(&packets[PACKET6], 2,READ_INPUT_REGISTERS , 86,2, 11);    // 30089-90 Sys Power(W) reg[10-11]
  modbus_construct(&packets[PACKET7], 2,READ_INPUT_REGISTERS , 226,2, 13);    // 30137-138 Frequency reg[12-13]     // HB : 16817 LB: 23593
  
  modbus_construct(&packets[PACKET8], 1, READ_HOLDING_REGISTERS, 4, 4, 15);  // Control Panel  
  //modbus_construct(&packets[PACKET2], 1, PRESET_MULTIPLE_REGISTERS, 0, 10, 0);
  
  modbus_configure(&Serial2, baud, SERIAL_8N1, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS, regs);
  delay(100);
  pinMode(LED, OUTPUT);
}

void loop()
{
  modbus_update();
  
  //regs[0] = analogRead(0)/4;
  for(int i=0;i<TOTAL_NO_OF_REGISTERS;i++)
  {
    Serial.println(regs[i],DEC); // PRINT SLAVE DATA
    //Serial.println(regs[PACKET2 + i]); // PRINT SLAVE DATA 
    delay(10);  
  }
  Serial.println("done");

  //delay(100);
  
  //analogWrite(LED, regs[0]>>2); 
}
