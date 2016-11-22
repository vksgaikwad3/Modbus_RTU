/* Description : Read all the Data from MODBUS Slaves.
   Hardware Connections : Arduino MEGA SERIAL2(Hardware Serial) 
                          RX2 -----> R0 (MAX485 Chip Adapter)
                          TX2 -----> DI (MAX485 Chip Adapter)
                          D2  -----> DE/RE (MAX485 Chip Adapter)
   MODBUS  Device Settings : 

   SLAVE 1 : Milk Chiller Panel 
   Slave ID : 1, Baud Rate : 9600, Parity : NONE, Stop bit : 1

   SLAVE 2 : Energy Meter
   Slave ID : 2, Baud Rate: 9600, Parity :None, Stop bit : 1
 * 
 * MODBUS Electrical Connections: 
 * 1> MILK Chiller Serial(RS 232) --> 485 Converter Pin 1 --> A of MAX485 Chip Adapter.
 *                                    485 Converter Pin 2 --> B of MAX485 Chip Adapter.        
 * 2> Energy Meter (RS485 Signal) --> A (Green Wire) -->  B of MAX485 Chip Adapter. 
 *                                    B (Yellow Wire) --> A of MAX485 Chip Adapter.
 *   
 *   
 */


#include <SimpleModbusMaster.h>

#define baud 9600
#define timeout 1000
#define polling 200
#define retry_count 20

#define TxEnablePin 2 

#define LED 13

#define TOTAL_NO_OF_REGISTERS 21        // Control Panel Resisters = 17 , Energy Meter Resisters = 16

enum
{
  PACKET1,
  PACKET2,
  PACKET3,
  //PACKET4,
  //PACKET5,
  //PACKET6,
  //PACKET7,
  //PACKET8,
  TOTAL_NO_OF_PACKETS // leave this last entry
};

Packet packets[TOTAL_NO_OF_PACKETS];

unsigned int regs[TOTAL_NO_OF_REGISTERS];     // All the Modbus Resisters gets stored here in buffer.

void setup()
{
  Serial.begin(9600);
  //READ_INPUT_REGISTERS
  //READ_HOLDING_REGISTERS
   //MILK CHILLER PACKET 
   modbus_construct(&packets[PACKET1], 1, READ_HOLDING_REGISTERS, 4, 17, 0);  // Control Panel Resisters [40005 - 40021]
  
   // ENERGY METER PACKET
    
    modbus_construct(&packets[PACKET2], 2,READ_INPUT_REGISTERS , 0, 2, 17);    //30001-2 Line Voltage reg[17-18]       HB :17278 LB :44628
    modbus_construct(&packets[PACKET3], 2,READ_INPUT_REGISTERS , 6, 2, 2);    // 30007-8 Line Current reg[19-20]      
 // modbus_construct(&packets[PACKET3], 2,READ_INPUT_REGISTERS , 42,2, 4);    // 30043-44 Device voltage reg[4-5]
  //modbus_construct(&packets[PACKET4], 2,READ_INPUT_REGISTERS , 46,2, 6);    // 30047-48 Device Current reg[6-7]
  //modbus_construct(&packets[PACKET5], 2,READ_INPUT_REGISTERS , 72,4, 8);    // 30073-74 Power Consumption  reg[8-9]   Reading 16384(73)[IMP]

  //modbus_construct(&packets[PACKET6], 2,READ_INPUT_REGISTERS , 86,2, 11);    // 30089-90 Sys Power(W) reg[10-11]
  //modbus_construct(&packets[PACKET7], 2,READ_INPUT_REGISTERS , 226,2, 13);    // 30137-138 Frequency reg[12-13]     // HB : 16817 LB: 23593
  
   
  //modbus_construct(&packets[PACKET2], 1, PRESET_MULTIPLE_REGISTERS, 0, 10, 0);
  
  modbus_configure(&Serial2, baud, SERIAL_8N1, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS, regs);
  delay(100);
  pinMode(LED, OUTPUT);
}

void loop()
{
  modbus_update();
  
  //regs[0] = analogRead(0)/4;
  //for(int i=0;i<TOTAL_NO_OF_REGISTERS;i++)
  //{
  //  Serial.println(regs[i],DEC); // PRINT SLAVE DATA
  //  delay(10);  
 // }
  Serial.print("Battery Temp.: ");Serial.println(regs[0]/10.0,DEC);
  Serial.print("Milk Temp.: ");Serial.println(regs[1]/10.0,DEC);
  Serial.print("Auxillary Temp.: ");Serial.println(regs[2]/10.0,DEC);
  Serial.print("Battery Volt.: ");Serial.println(regs[3],DEC);
  Serial.print("AC Voltage.: ");Serial.println(regs[4],DEC);
  Serial.print("Copmressor Curr.: ");Serial.println(regs[5]/10.0,DEC);
  Serial.print("Pump Current.: ");Serial.println(regs[6]/10.0,DEC);
  Serial.print("Ch Pump Relay.: ");Serial.println(regs[7],DEC);
  Serial.print("Condensor Relay.: ");Serial.println(regs[8],DEC);
  Serial.print("Compressor Relay.: ");Serial.println(regs[9],DEC);
  Serial.print("Inverter Relay.: ");Serial.println(regs[10],DEC);
  Serial.print("Agitator Relay.: ");Serial.println(regs[11],DEC);
  Serial.print("Tank Relay.: ");Serial.println(regs[12],DEC);
  Serial.print("Shiva Relay.: ");Serial.println(regs[13],DEC);
  //Serial.print("Discharge Pump Relay.: ");Serial.println(regs[14],DEC);
  Serial.print("Discharge Pump Relay.: ");Serial.println(regs[14],DEC);
  Serial.print("Compressor Run Hr.: ");Serial.println(regs[16],DEC);
  delay(20);

  // Energy Meter Readings 
  Serial.println("************ Energy Meter Readings ************* ");
  
  Serial.print("Line Voltages[HB].: ");Serial.println(regs[17],HEX);
  Serial.print("Line Voltages[LB].: ");Serial.println(regs[18],HEX);
  Serial.print("Line Current [HB].: ");Serial.println(regs[19],HEX);
  Serial.print("Line Current [LB].: ");Serial.println(regs[20],HEX);


  delay(20);
  
  Serial.println("**** done ****");

  
}
