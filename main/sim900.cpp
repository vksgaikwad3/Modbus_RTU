#include "sim900.h"


// GSM/GPRS Configurations and Think Spwak Cloud Configurations Settings

char aux_str[100];
    
char pin[]="";
//char apn[]="imis/internet";     //idea
char apn[]="airtelgprs.com";     //Airtel

char user_name[]="";
char password[]="";

String getStr="";

char IP_address[]="184.106.153.149";   //Think Speak Server IP Address
char port[]= "80";                      // PORT Connected on 

boolean sim900Status = false;

/*************************************************************************************************/

sim900_GPRS::sim900_GPRS()
{
    //power_on();     // GSM ON
    
}

/*************************************************************************************************/

void sim900_GPRS::power_on()
{
   uint8_t answer=0;
    
   sim900Status=true;
   int ctT=0;
 digitalWrite(13,HIGH);
 delay(1000);
 digitalWrite(13,LOW);
 delay(2000);
 digitalWrite(13,HIGH);
 delay(5000);
    // checks if the module is started
    answer = sendATcommand("AT", "OK", 2000);
    if (answer == 0)
    {
        // waits for an answer from the module
        while(answer == 0){     // Send AT every two seconds and wait for the answer
            answer = sendATcommand("AT", "OK", 2000);    
      ctT++;
    if(ctT==10){
     sim900Status=true;
     Serial.println("GSM OFF");
    power_on();
    }  
      }
    }
    Serial.println("GSM ON Sucessfully");
    // Once GSM get ON dothe folowing configurations to Send Systems Setting over SMS
      
    //sendATcommand("ATE0", "OK", 1000);        // ON/OFF AT command info
   // sendATcommand("AT+CMGF=1", "OK",1000);    // Select SMS message Format 0 = PDU mode , 1 = Text Mode
   // sendATcommand("AT+CMGD=1", "OK", 1000);   // Delete an SMS message
   // sendATcommand("AT+CNMI=2,2,0,0,0\r", "OK", 1000);  //New SMS Indications 
   // sendATcommand("AT+CMGR=1", "OK", 1000);            // Read SMS message has just arrived
    
    
   // sendATcommand("AT+CCLK?", "OK", 1000);          // GSM Clock optional
  //   sendATcommand("AT+CCLK=\"16/07/27,14:32:30+05\"", "OK", 1000);
  //  sendSMS("hello");
}

/******************************************************************************************************************************************/

int8_t sim900_GPRS::sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout)
{

    uint8_t x=0,  answer=0;
    char response[100];         //Response buffer
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( Serial.available() > 0) Serial.read();    // Clean the input buffer

    Serial.println(ATcommand);    // Send the AT command 
    Serial1.println(ATcommand); //Response from Serial1 

    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        if(Serial1.available() != 0){    
            // if there are data in the UART input buffer, reads it and checks for the asnwer
            response[x] = Serial1.read();     //Read Response from Serial1 port
            Serial.print(response[x]);        //Print response on Serial 0 port
            x++;
            // check if the desired answer  is in the response of the module
            if (strstr(response, expected_answer) != NULL)    
            {
                answer = 1;
            }
        }
    }
    // Waits for the asnwer with time out
    while((answer == 0) && ((millis() - previous) < timeout));    //Check till answer = 0 and timout period(ms)

        return answer;
}

/**********************************************************************************************************************************************************/

int8_t sim900_GPRS::sendATcommand2(String ATcommand, char* expected_answer1,char* expected_answer2, unsigned int timeout)
{
  
    uint8_t x=0,  answer=0;
   char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( Serial1.available() > 0) Serial1.read();    // Clean the input buffer

  Serial1.println(ATcommand);    // Send the AT command 
Serial.println(ATcommand);
    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        // if there are data in the UART input buffer, reads it and checks for the asnwer
        if(Serial1.available() != 0){    
            response[x] = Serial1.read();
             Serial.print(response[x]);
            x++;
            // check if the desired answer 1  is in the response of the module
            if (strstr(response, expected_answer1) != NULL)    
            {
                answer = 1;
            }
            // check if the desired answer 2 is in the response of the module
            else if (strstr(response, expected_answer2) != NULL)    
            {
                answer = 2;
            }
          
         
        }
    }
    // Waits for the asnwer with time out
    while((answer == 0) && ((millis() - previous) < timeout));    

    return answer;
}

/**************************************************************************************************************************************************************/

int8_t sim900_GPRS::sendATcommand3(String ATcommand, char* expected_answer1,char* expected_answer2,char*expected_answer3, unsigned int timeout)
{

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( Serial1.available() > 0) Serial1.read();    // Clean the input buffer

    Serial1.println(ATcommand);    // Send the AT command 
    Serial.println(ATcommand);
    x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        // if there are data in the UART input buffer, reads it and checks for the asnwer
        if(Serial1.available() != 0){    
            response[x] = Serial1.read();
             Serial.print(response[x]);
            x++;
            // check if the desired answer 1  is in the response of the module
            if (strstr(response, expected_answer1) != NULL)    
            {
                answer = 1;
            }
            // check if the desired answer 2 is in the response of the module
            if (strstr(response, expected_answer2) != NULL)    
            {
                answer = 2;
            }
            else if (strstr(response, expected_answer3) != NULL)    
            {
                answer = 3;
            }
          
         
        }
    }
    // Waits for the asnwer with time out
    while((answer == 0) && ((millis() - previous) < timeout));    

    return answer;
}

/*****************************************************************************************************************************************************/

void sim900_GPRS::updateThinkSpeak(String channel_apiKey,uint8_t id1,uint8_t id2,uint8_t id3,uint8_t id4,float field1=0,float field2=0,float field3=0,float field4=0)  // ,float field5=0,float field6=0,float field7=0,float field8=0)
{
  
   if(sim900Status==true)
   {  // Selects Single-connection mode
      if (sendATcommand2("AT+CIPMUX=0", "OK","ERROR", 1000) == 1)      // CIMPUX=0 is already set in Single-connection mode
       {
        // Waits for status IP INITIAL
          while(sendATcommand("AT+CIPSTATUS","INITIAL", 1000)  == 0 );  // Check Current Connection Status
          delay(5000);                                                   //wait 5 sec
        //aux_str="AT+CSTT=\"airtelgprs.com\",\"\",\"\"";
        //sendATcommand2(aux_str, "CONNECT OK", "CONNECT FAIL", 30000);
          snprintf(aux_str, sizeof(aux_str), "AT+CSTT=\"%s\",\"%s\",\"%s\"", apn, user_name, password);  //Put GPRS setings
        // Sets the APN, user name and password
        if (sendATcommand2(aux_str, "OK",  "ERROR", 30000) == 1)
        {            
          // Waits for status IP START
          while(sendATcommand("AT+CIPSTATUS", "START", 500)  == 0 );
          delay(2000);
          // Brings Up Wireless Connection
          if (sendATcommand2("AT+CIICR", "OK", "ERROR", 30000) == 1)
          {    
              //delay(2000);
              Serial.println("\n Bringup Wireless Connection ...........");
             // Waits for status IP GPRSACT
             while(sendATcommand("AT+CIPSTATUS", "GPRSACT", 1000)  == 0 );
             delay(2000);
            
              // Gets Local IP Address
              if (sendATcommand2("AT+CIFSR", ".", "ERROR", 10000) == 1)
              {
                 // Waits for status IP STATUS
                 while(sendATcommand("AT+CIPSTATUS", "IP STATUS", 500)  == 0 );
                 delay(2000);
                 Serial.println("Opening TCP");
                 //  aux_str="AT+CIPSTART=\"TCP\",\"184.106.153.149\",\"80\"";
                 //  sendATcommand2(aux_str, "CONNECT OK", "CONNECT FAIL", 30000);
                 snprintf(aux_str, sizeof(aux_str), "AT+CIPSTART=\"TCP\",\"%s\",\"%s\"", IP_address, port);
                 // snprintf(aux_str, sizeof(aux_str), "AT+CIPSTART=\"TCP\",\"%s\",\"%s\"", thethingsiOserver,port);  // thethings.iO Server   
                // Opens a TCP socket
                 if (sendATcommand2(aux_str, "CONNECT OK", "CONNECT FAIL", 30000) == 1)
                 {
                         Serial.println("Connected");

                         Serial.println("\n Send some data to TCP Socket............");
                         getStr="GET /update?api_key="+ channel_apiKey +"&field"+ id1 +"=" +field1 + "&field"+ id2 + "=" +field2 +"&field" +id3 + "=" +field3 +"&field" +id4 +"=" +field4 
                                                      /*+"&field5="+ field5  +"&field6="+ field6 +"&field7="+ field7 +"&field8=" + field8*/ + "\r\n\r\n";      //TCP packet to send GET Request on https (Think Speak)
                          
                          String sendcmd = "AT+CIPSEND="+ String(getStr.length());
                          if (sendATcommand2(sendcmd, ">", "ERROR", 10000) == 1)    
                            {
                              delay(100);
                              //ThingSpeak.writeField(Channel_ID, 1, field1, myWriteAPIKey);
                              sendATcommand2(getStr, "SEND OK", "ERROR", 10000);      //Field1 Data sending from here
                              delay(100);
                            }
                            Serial.println("Closing the Socket............");
                            
                            
                             // Closes the socket
                             sendATcommand2("AT+CIPCLOSE", "CLOSE OK", "ERROR", 20000);
                             Serial.println("Conneection CLOSED OK");
                  }
                  else
                  {
                      Serial.println("Error opening the connection");
                       
                  }  
              }
          }
        }
       }
   }
    Serial.println("Shutting down the connection.........");
    sendATcommand2("AT+CIPSHUT", "OK", "ERROR", 10000);
    delay(5000);
}


/**********************************************************************************************************************************************************/
void sim900_GPRS::updateMilkChillerRelays(String channel_apiKey,uint8_t field1,uint8_t field2,uint8_t field3,uint8_t field4,uint8_t field5,uint8_t field6,uint8_t field7,uint8_t field8)
{
   if(sim900Status==true)
   {  // Selects Single-connection mode
      if (sendATcommand2("AT+CIPMUX=0", "OK","ERROR", 1000) == 1)      // CIMPUX=0 is already set in Single-connection mode
       {
        // Waits for status IP INITIAL
          while(sendATcommand("AT+CIPSTATUS","INITIAL", 1000)  == 0 );  // Check Current Connection Status
          delay(5000);                                                   //wait 5 sec
        //aux_str="AT+CSTT=\"airtelgprs.com\",\"\",\"\"";
        //sendATcommand2(aux_str, "CONNECT OK", "CONNECT FAIL", 30000);
          snprintf(aux_str, sizeof(aux_str), "AT+CSTT=\"%s\",\"%s\",\"%s\"", apn, user_name, password);  //Put GPRS setings
        // Sets the APN, user name and password
        if (sendATcommand2(aux_str, "OK",  "ERROR", 30000) == 1)
        {            
          // Waits for status IP START
          while(sendATcommand("AT+CIPSTATUS", "START", 500)  == 0 );
          delay(2000);
          // Brings Up Wireless Connection
          if (sendATcommand2("AT+CIICR", "OK", "ERROR", 30000) == 1)
          {    
              //delay(2000);
              Serial.println("\n Bringup Wireless Connection ...........");
             // Waits for status IP GPRSACT
             while(sendATcommand("AT+CIPSTATUS", "GPRSACT", 1000)  == 0 );
             delay(2000);
            
              // Gets Local IP Address
              if (sendATcommand2("AT+CIFSR", ".", "ERROR", 10000) == 1)
              {
                 // Waits for status IP STATUS
                 while(sendATcommand("AT+CIPSTATUS", "IP STATUS", 500)  == 0 );
                 delay(2000);
                 Serial.println("Opening TCP");
                 //  aux_str="AT+CIPSTART=\"TCP\",\"184.106.153.149\",\"80\"";
                 //  sendATcommand2(aux_str, "CONNECT OK", "CONNECT FAIL", 30000);
                 snprintf(aux_str, sizeof(aux_str), "AT+CIPSTART=\"TCP\",\"%s\",\"%s\"", IP_address, port);
                 // snprintf(aux_str, sizeof(aux_str), "AT+CIPSTART=\"TCP\",\"%s\",\"%s\"", thethingsiOserver,port);  // thethings.iO Server   
                // Opens a TCP socket
                 if (sendATcommand2(aux_str, "CONNECT OK", "CONNECT FAIL", 30000) == 1)
                 {
                         Serial.println("Connected");

                         Serial.println("\n Send some data to TCP Socket............");
                         getStr="GET /update?api_key="+ channel_apiKey +"&field1="+ field1 + "&field2="+ field2 +"&field3=" +field3 +"&field4=" +field4 
                                                      +"&field5="+ field5  +"&field6="+ field6 +"&field7="+ field7 +"&field8=" + field8 + "\r\n\r\n";      //TCP packet to send GET Request on https (Think Speak)
                          
                          // getStr= "GET /raspberry/api.php?serviceName=test_api&field1=123\r\n\r\n";  //+ "\r\n\r\n";   //Smartnub packet request
                          //String value = "{"values": [ { "key": "fun","value": "5000" } ] }\";
                          //getStr="POST /v2/things/{{doAgkdNly8o29pusozJ-3mNnqET6dKKvcI4DmLVe-t4}}\" + value + "\r\n\r\n";   //thethingsIO request to write
                          
                          //getStr="PUT https://api.thingspeak.com/channels/"+ Channel_ID ;
                         // getStr += apiKey;
                         // getStr +="&field1=";
                         // getStr += field1;
                         // getStr += "\r\n\r\n";
                          
                          String sendcmd = "AT+CIPSEND="+ String(getStr.length());
                          if (sendATcommand2(sendcmd, ">", "ERROR", 10000) == 1)    
                            {
                              delay(100);
                              //ThingSpeak.writeField(Channel_ID, 1, field1, myWriteAPIKey);
                              sendATcommand2(getStr, "SEND OK", "ERROR", 10000);      //Field1 Data sending from here
                              delay(100);
                            }
                            Serial.println("Closing the Socket............");
                            
                            
                             // Closes the socket
                             sendATcommand2("AT+CIPCLOSE", "CLOSE OK", "ERROR", 20000);
                             Serial.println("Conneection CLOSED OK");
                  }
                  else
                  {
                      Serial.println("Error opening the connection");
                       
                  }  
              }
          }
        }
       }
   }
    Serial.println("Shutting down the connection.........");
    sendATcommand2("AT+CIPSHUT", "OK", "ERROR", 10000);
    delay(5000);
}


/**********************************************************************************************************************************************************/

float sim900_GPRS:: hextofloat(uint16_t hByte, uint16_t lByte)
{
  uint32_t hb = 0x0000FFFF & hByte ; 
  uint32_t num = (hb << 16) | lByte ;
  
   //uint32_t lb = Serial.println(44628);
  
  uint32_t myString[1] ={num } ;
  uint32_t val;
  float floatVal;
  
  //Serial.print("Masking No :");Serial.println(hb);
  //Serial.print("After Shift << 16: ");Serial.println(num);
  num = myString[0];
  //Serial.println(num);
  floatVal = *((float*)&num);            // OR floatVal = *((float*)&myString);
  //Serial.println(floatVal);

  return floatVal; 
}


