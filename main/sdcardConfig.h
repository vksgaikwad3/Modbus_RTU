#ifndef sdcardconfig_h
#define sdcardconfig_h

#include <SPI.h>
#include <SD.h>

File myFile;

void isSDCardCheck(String filename);
void readSDCard(String filename);
boolean writetoSDCard (String filename,byte mode,int srNo,int deviceID,float battery_Temp,float milk_Temp,float auxillary_Temp,float battery_Volt,float ac_Volt,float compressor_Current,float pump_Current,
                       boolean charg_pump_Relay,boolean condensor_Relay,boolean compressor_Relay,boolean inverter_Relay,boolean agitator_Relay,boolean tank_Relay,boolean shiva_Relay,boolean discharge_pump_Relay,float compressor_run_Hour );




void isSDCardCheck(String filename)
{
  
  Serial.print("Initializing SD card...");

  while (SD.begin(10) ==0 ) {
    Serial.println("initialization failed!  NO SD CARD FOUND :( InsertCorrectly !!! ");
    continue;
    //return;
  }
  Serial.println("initialization done.");

/*** Checking if File Exist or not *******************/

  if (SD.exists(filename)) {
    Serial.println("chiller.csv exists.");
    myFile = SD.open("chiller.csv", FILE_WRITE);
    if(myFile.size() <= 0)    //check Size of file is zero or not
    {
     Serial.println("Start Writting Tabs !!! .");
     myFile.println("SrNo.,DeviceID,BatteryTemp(oC),MilkTemp(oC),AuxillaryTemp(oC),BatteryVoltage(VDC),ACVoltage(ACV),CompresorCurrent(A),PumpCurrent(A),ChargingPumpRelay,CondensorRelay,CompressorRelay,InverterRelay,AgitatorRelay,TankRelay,ShivaRelay,Disch.PumpRelay,CompressorRunHours ");
     myFile.close();
    }
    
    //SD.remove("chiller.csv");
  } 
  else if(myFile != 1) 
  {
    Serial.println("chiller.csv doesn't exist. Creating a New .csv file");
  

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  
  myFile = SD.open(filename, FILE_WRITE);

  // if the file opened okay, write to it:
  while(myFile != 1)
   {
    myFile = SD.open(filename, FILE_WRITE);
    Serial.println("Trying to OPEN File and Create ");
    continue;
   }
    Serial.print("IN File Status:");Serial.println(myFile);
    
    delay(1000);
    Serial.print("STATE:");Serial.println(myFile);
    //if(myFile.size() <= 0)        //Check if Data is available or not
    //{
      Serial.print("File Size:");Serial.println(myFile.size());
      Serial.print("Create Tabs in .csv...");
      myFile.println("SrNo.,DeviceID,BatteryTemp(oC),MilkTemp(oC),AuxillaryTemp(oC),BatteryVoltage(VDC),ACVoltage(ACV),CompresorCurrent(A),PumpCurrent(A),ChargingPumpRelay,CondensorRelay,CompressorRelay,InverterRelay,AgitatorRelay,TankRelay,ShivaRelay,Disch.PumpRelay,CompressorRunHours ");
      myFile.close();
      Serial.println("Write Complete .");
   } 

/******* Reading .CSV File ******************/ 
  readSDCard(filename);

}

void readSDCard(String filename)
{
    
/******* Reading .CSV File ******************/ 
  // re-open the file for reading:
  myFile = SD.open(filename);       //default in READ mode
  if (myFile) {
    Serial.print("File OPEN :");Serial.println(filename);

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
      
    }
    Serial.println("Read Complete");
    // close the file:
    myFile.close();
    Serial.println("*******************File Closed ******************************");
  } else {
    // if the file didn't open, print an error:
    myFile = SD.open(filename, FILE_WRITE);  
    while(myFile !=1)
    {
      myFile = SD.open(filename);    // Try to Open File
      Serial.println("error opening and Reading chiller.csv, ... Trying to Open file");
      Serial.println(myFile);
      //continue;
      
    }
    
   }
}


/********************** Write Data to SD Card  ********************************/

boolean writetoSDCard (String filename,byte mode,int srNo,int deviceID,float battery_Temp,float milk_Temp,float auxillary_Temp,float battery_Volt,float ac_Volt,float compressor_Current,float pump_Current,
                       boolean charg_pump_Relay,boolean condensor_Relay,boolean compressor_Relay,boolean inverter_Relay,boolean agitator_Relay,boolean tank_Relay,boolean shiva_Relay,boolean discharge_pump_Relay,float compressor_run_Hour )
{
  String data ;
  //volatile int srNo = 1;

  
  myFile = SD.open(filename, mode); // FILE_WRITE

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Write data in MilkChiller.csv...");
    //data = "1,1," + battery_Temp + "," + milk_Temp + "," + auxillary_Temp + "," + battery_Volt + "," + ac_Volt +"," + compressor_Current +"," + pump_Current +","; 
    //+ charg_pump_Relay +"," + condensor_Relay +"," + compressor_Relay + "," + inverter_Relay +"," + agitator_Relay +"," + tank_Relay + "," + shiva_Relay +"," + discharge_pump_Relay +"," + compressor_run_Hour + """;
    data +=  srNo;
    data += ",";
    data += deviceID;
    data += "," ;
    data += battery_Temp;
    data += ",";
    data += milk_Temp;
    data += ",";
    data += auxillary_Temp;
    data += ",";
    data += battery_Volt;
    data +=  "," ;
    data += ac_Volt ;
    data += "," ;
    data += compressor_Current;
    data += "," ;
    data += pump_Current ;
    data += "," ;
    data += charg_pump_Relay ;
    data += "," ;
    data += condensor_Relay ;
    data += "," ;
    data += compressor_Relay ;
    data += "," ;
    data += inverter_Relay ;
    data += "," ; 
    data += agitator_Relay ;
    data += "," ; 
    data += tank_Relay;
    data += "," ;
    data += shiva_Relay ;
    data += "," ;   
    data += discharge_pump_Relay ;
    data += "," ;
    data += compressor_run_Hour;
     
    //srNo++;
    Serial.println(data);
    delay(2000);
    myFile.println(data);
    
    myFile.close();
    
    Serial.println("Write Complete .");
    return true;
  } 
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening chiller.csv");
    
  }

  return false;
}




#endif
