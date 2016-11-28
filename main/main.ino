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
#include <SPI.h>
#include <SD.h>
#include "sim900.h"     
#include "serverConfg.h"  
#include "sdcardConfig.h"        

//File myFile1;
sim900_GPRS myGateway;    // Gateway object

#define baud 9600
#define timeout 1000
#define polling 200
#define retry_count 20

#define TxEnablePin 2 

#define LED 13
const uint8_t ChipSelect = 10 ;   //SD Card ChipSelect pin. Dont Change


#define TOTAL_NO_OF_REGISTERS 33        // Control Panel Resisters = 17 , Energy Meter Resisters = 16

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
  PACKET9,
  TOTAL_NO_OF_PACKETS // leave this last entry
};

Packet packets[TOTAL_NO_OF_PACKETS];

int regs[TOTAL_NO_OF_REGISTERS];     // All the Modbus Resisters gets stored here in buffer.

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600);
  //READ_INPUT_REGISTERS
  //READ_HOLDING_REGISTERS
   //MILK CHILLER PACKET 
   modbus_construct(&packets[PACKET1], 1, READ_HOLDING_REGISTERS, 4, 17, 0);  // Control Panel Resisters [40005 - 40021]
  
   // ENERGY METER PACKET
    
    modbus_construct(&packets[PACKET2], 2,READ_INPUT_REGISTERS , 0, 2, 17);    //30001-2 Line Voltage reg[17-18]       HB :17278 LB :44628
    modbus_construct(&packets[PACKET3], 2,READ_INPUT_REGISTERS , 6, 2, 19);    // 30007-8 Line Current reg[19-20]      
    modbus_construct(&packets[PACKET4], 2,READ_INPUT_REGISTERS , 42,2, 21);    // 30043-44 Device voltage reg[21-22]
    modbus_construct(&packets[PACKET5], 2,READ_INPUT_REGISTERS , 46,2, 23);    // 30047-48 Device Current reg[23-24]
    modbus_construct(&packets[PACKET6], 2,READ_INPUT_REGISTERS , 72,4, 25);    // 30073-74 Power Consumption  reg[25-26]   Reading 16384(73)[IMP]

    modbus_construct(&packets[PACKET7], 2,READ_INPUT_REGISTERS , 86,2, 27);    // 30089-90 Sys Power(W) reg[27-28]
    modbus_construct(&packets[PACKET8], 2,READ_INPUT_REGISTERS , 226,2,29);    // 30227-28 Device Run Hr reg[29-30]     
    modbus_construct(&packets[PACKET9], 2,READ_INPUT_REGISTERS , 228,2,31);    // 30227-28 Power Available Time reg[31-32]    
    
   
  //modbus_construct(&packets[PACKET2], 1, PRESET_MULTIPLE_REGISTERS, 0, 10, 0);
  
  modbus_configure(&Serial2, baud, SERIAL_8N1, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS, regs);
  delay(100);
  //pinMode(LED, OUTPUT);

  // Setup GSM Module 
  Serial1.begin(9600);   // To connect SIM900A and send AT Commands
  myGateway.power_on();            // POWER ON GSM Module for communication 

/************** SD Card Init/Startup Code ***********************/
 isSDCardCheck("chiller.csv");     //provide a File Name to Store log of ModBus Devices

  
}

void loop()
{
  for(int i=0;i<200;i++)
  {
    modbus_update();
    delay(10);Serial.println(i);
  }
 
  volatile float battery_Temp = regs[0]/10.0;
  volatile float milk_Temp = regs[1]/10.0;
  volatile float auxillary_Temp = regs[2]/10.0;
  volatile uint8_t battery_Volt = regs[3];
  volatile uint8_t ac_Volt = regs[4];
  volatile float compressor_Current = regs[5]/10.0;
  volatile float pump_Current = regs[6]/10.0;
  volatile boolean charg_pump_Relay = regs[7];
  volatile boolean condensor_Relay = regs[8];
  volatile boolean compressor_Relay = regs[9];
  volatile boolean inverter_Relay = regs[10];
  volatile boolean agitator_Relay = regs[11];
  volatile boolean tank_Relay = regs[12];
  volatile boolean shiva_Relay = regs[13];
  volatile boolean discharge_pump_Relay = regs[14];
  volatile uint32_t compressor_run_Hour = regs[16];
     
  Serial.print("Battery Temp.: ");Serial.println(battery_Temp,DEC);
  Serial.print("Milk Temp.: ");Serial.println(milk_Temp,DEC);
  Serial.print("Auxillary Temp.: ");Serial.println(auxillary_Temp,DEC);
  Serial.print("Battery Volt.: ");Serial.println(battery_Volt,DEC);
  Serial.print("AC Voltage.: ");Serial.println(ac_Volt,DEC);
  Serial.print("Copmressor Curr.: ");Serial.println(compressor_Current,DEC);
  Serial.print("Pump Current.: ");Serial.println(pump_Current,DEC);
  Serial.print("Ch Pump Relay.: ");Serial.println(charg_pump_Relay,DEC);
  Serial.print("Condensor Relay.: ");Serial.println(condensor_Relay,DEC);
  Serial.print("Compressor Relay.: ");Serial.println(compressor_Relay,DEC);
  Serial.print("Inverter Relay.: ");Serial.println(inverter_Relay,DEC);
  Serial.print("Agitator Relay.: ");Serial.println(agitator_Relay,DEC);
  Serial.print("Tank Relay.: ");Serial.println(tank_Relay,DEC);
  Serial.print("Shiva Relay.: ");Serial.println(shiva_Relay,DEC);
  //Serial.print("Discharge Pump Relay.: ");Serial.println(regs[14],DEC);
  Serial.print("Discharge Pump Relay.: ");Serial.println(discharge_pump_Relay,DEC);
  Serial.print("Compressor Run Hr.: ");Serial.println(compressor_run_Hour,DEC);
  delay(20);
 
    // Energy Meter Readings 
  Serial.println("************ Energy Meter Readings ************* ");
  /*
  Serial.print("Line Voltages[HB].: ");Serial.println(regs[17],HEX);
  Serial.print("Line Voltages[LB].: ");Serial.println(regs[18],HEX);
  Serial.print("Line Current [HB].: ");Serial.println(regs[19],HEX);
  Serial.print("Line Current [LB].: ");Serial.println(regs[20],HEX);
  Serial.print("Device Voltage [HB].: ");Serial.println(regs[21],HEX);
  Serial.print("DEvice Voltage [LB].: ");Serial.println(regs[22],HEX);
  Serial.print("Device Current [HB].: ");Serial.println(regs[23],HEX);
  Serial.print("DEvice Current [LB].: ");Serial.println(regs[24],HEX);
  Serial.print("POWER[Sys W] [HB].: ");Serial.println(regs[25],HEX);
  Serial.print("POWER[Sys W] [LB].: ");Serial.println(regs[26],HEX);
  Serial.print("Power Consumption [HB].: ");Serial.println(regs[27],HEX);
  Serial.print("Power Consumption [LB].: ");Serial.println(regs[28],HEX);
  Serial.print("Device Run Hr [HB].: ");Serial.println(regs[29],HEX);
  Serial.print("DEvice Run Hr [LB].: ");Serial.println(regs[30],HEX);
  Serial.print("Power Available Time [HB].: ");Serial.println(regs[31],HEX);
  Serial.print("Power Available Time [LB].: ");Serial.println(regs[32],HEX);
  */
  delay(50);
  float lineVolts = myGateway.hextofloat(regs[17],regs[18]);
  float lineCurrent = myGateway.hextofloat(regs[19],regs[20]);
  float deviceVolts = myGateway.hextofloat(regs[21],regs[22]);
  float deviceCurrent = myGateway.hextofloat(regs[23],regs[24]);
  float power = myGateway.hextofloat(regs[25],regs[26]);
  float powerConsump = myGateway.hextofloat(regs[27],regs[28]);
  float deviceRunHr = myGateway.hextofloat(regs[29],regs[30]);
  float powerAvailable = myGateway.hextofloat(regs[31],regs[32]);

/* *********  Log Data to SD Card *********************************/
  String data = "";
  volatile int SrNo = 1;
  //myFile1 = SD.open("chiller.csv", FILE_WRITE);

 writetoSDCard ("chiller.csv",FILE_WRITE ,SrNo, 2, battery_Temp, milk_Temp, auxillary_Temp, battery_Volt, ac_Volt, compressor_Current, pump_Current,
                 charg_pump_Relay, condensor_Relay, compressor_Relay, inverter_Relay, agitator_Relay,tank_Relay, shiva_Relay, discharge_pump_Relay,compressor_run_Hour );

  
  SrNo++;
  
  float emarray[] = {lineVolts,lineCurrent,deviceVolts,deviceCurrent,power,powerConsump/1000,deviceRunHr,powerAvailable };
  for(int i=0;i<8;i++)
  {
    Serial.println(emarray[i]);
    delay(1);
    
  }
  
  delay(5000); 
  /******************** Milk Chiller Temp Data Uploading *****************************/
  int stime = millis();
  myGateway.updateThinkSpeak(channel_apiKey[0],1,2,3,4,battery_Temp,milk_Temp,auxillary_Temp,battery_Volt,ac_Volt,compressor_Current,pump_Current,compressor_run_Hour);    //update Milk Chiller Channel 1 - 4 fields
  Serial.print("Chiller 1-4 Time Taken :");Serial.println(millis() - stime );
  delay(200);
  //int s1time = millis();
  //myGateway.updateThinkSpeak(channel_apiKey[0],5,6,7,8,ac_Volt,compressor_Current,pump_Current,compressor_run_Hour);    //update Milk Chiller Channel field 5 -8
  //Serial.print("Chiller 5 -8 Time Taken :");Serial.println(millis() - s1time );
  //delay(200);

 /****************** Milk Chiller Relays Data Uploading ***************************/ 
  int s2time = millis();
  //myGateway.updateMilkChillerRelays(channel_apiKey[1],charg_pump_Relay,condensor_Relay,compressor_Relay,inverter_Relay,agitator_Relay,tank_Relay,shiva_Relay,discharge_pump_Relay );   // pass the parameters of milkChiller Relay fields 1 - 4
  
  myGateway.updateThinkSpeak(channel_apiKey[1],1,2,3,4,charg_pump_Relay,condensor_Relay,compressor_Relay,inverter_Relay,agitator_Relay,tank_Relay,shiva_Relay,discharge_pump_Relay);    //update Milk Chiller Relays Channel field 1 -4
  Serial.print("Chiller Relays 1-8 Time Taken :");Serial.println(millis() - s2time );
  delay(200);
 
  //int s3time = millis();
  //myGateway.updateThinkSpeak(channel_apiKey[1],5,6,7,8,agitator_Relay,tank_Relay,shiva_Relay,discharge_pump_Relay);    //update Milk Chiller Relays Channel field 5 - 8
  //Serial.print("Chiller Relays 1-8 Time Taken :");Serial.println(millis() - s3time );
  //delay(200);
  
/*********************** Energy Meter Uploaing ****************************************/  
  int s4time = millis();
  myGateway.updateThinkSpeak(channel_apiKey[2],1,2,3,4,lineVolts,lineCurrent,deviceVolts,deviceCurrent,power,powerConsump/1000,deviceRunHr,powerAvailable);                  //update Energy meter 1 - 4 fields
  Serial.print("Energy Meter 1-4 Time Taken :");Serial.println(millis() - s4time );
  delay(200);
  
 // int s5time = millis();
 // myGateway.updateThinkSpeak(channel_apiKey[2],5,6,7,8,power,powerConsump/1000,deviceRunHr,powerAvailable);               // Energy Meter 5 - 8 fields
 // Serial.print("Energy Meter 5 - 8 Time Taken :");Serial.println(millis() - s5time );

 
    //myGateway.updateThinkSpeak(channel_apiKey[2],1,2,3,4,lineVolts,lineCurrent,deviceVolts,deviceCurrent);                  //update Energy meter 1 - 4 fields  
  //  myGateway.updateThinkSpeak(channel_apiKey[0],1,2,3,4,100.79,20.70,30.20,440.40,4551.1545,87785.166,98544.4661,444551.1541);
 //   delay(1000);
    //myGateway.updateThinkSpeak(channel_apiKey[0],5,6,7,8,99999.1453,123456.4463,448454,763313.046);                  //update Energy meter 1 - 4 fields  
    //delay(500);
    
   //myGateway.updateThinkSpeak(channel_apiKey[2],5,6,7,8,power,powerConsump/1000,deviceRunHr,powerAvailable);                    //update Energy meter 5 - 8 fields
  //myGateway.updateThinkSpeak(channel_apiKey[2],5,6,7,8,41.1,912.24,901.14,9999);                                                //update Energy meter 5 - 8 fields
   
  Serial.println("**** done ****");
  delay(200);
 
}


