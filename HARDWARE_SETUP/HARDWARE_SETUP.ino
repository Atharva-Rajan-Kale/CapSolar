#include "MQ135.h"
MQ135 gasSensor = MQ135(A1);
const int solarPin = A5; // Analog pin connected to the solar panel
int solarValue = 0;
#include <SoftwareSerial.h>       //Software Serial library
SoftwareSerial espSerial(2, 3);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true
String mySSID = "kale";       // WiFi SSID
String myPWD = "atharva123"; // WiFi Password
String myAPI = "Y6OXOYFCL0ZGJZWK";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD1 = "field1"; 
String myFIELD2= "field2";
String myFIELD3 = "field3";
String myFIELD4 = "field4";
int sendVal;
const int BUZ = 6;
int sensorThres_1 = 10;
int sensorThres_2 = 100;
int sensorThres_3 = 150;


void setup()
{
  pinMode(BUZ, OUTPUT);
  Serial.begin(9600);
  espSerial.begin(115200);
  
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  /*while(!esp.find("OK
  {          
      //Wait for connection
  }*/
  delay(1000);
  
}

  void loop()
  {
    /* Here, I'm using the function random(range) to send a random value to the 
     ThingSpeak API. You can change this value to any sensor data
     so that the API will show the sensor data  
    */
    float ppm = gasSensor.getPPM();
    Serial.println(ppm);
  
    
    float NO = analogRead(A2);
    Serial.print(NO);
    Serial.println(",");
  
    
    float BEN = analogRead(A3);
    Serial.print(BEN);
    Serial.println(",");
  
    
    solarValue = analogRead(solarPin); // Read the analog value from the solar panel
    float voltage = solarValue * (5.0 / 1023.0); // Convert the analog value to voltage
    Serial.print(voltage);
    Serial.println(" ");

    
    if (ppm > sensorThres_1 || NO > sensorThres_2 || BEN > sensorThres_3){
      digitalWrite(BUZ, HIGH);
      }
    else{  
      digitalWrite(BUZ, LOW);
      }
    //delay(20000);   // Wait for 1 second before taking another reading

    
    //sendVal = random(1000); // Send a random number between 1 and 1000
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD1 +"="+String(ppm);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData);
    Serial.print("Value to be sent: ");
    Serial.println(ppm);
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(10000);
    
    String sendData1 = "GET /update?api_key="+ myAPI +"&"+ myFIELD2 +"="+String(NO);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData1.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData1);
    Serial.print("Value to be sent: ");
    Serial.println(NO);
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(10000);
    
    String sendData2 = "GET /update?api_key="+ myAPI +"&"+ myFIELD3 +"="+String(BEN);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData2.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData2);
    Serial.print("Value to be sent: ");
    Serial.println(BEN);
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(10000);
    
    String sendData3 = "GET /update?api_key="+ myAPI +"&"+ myFIELD4 +"="+String(voltage);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData3.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData3);
    Serial.print("Value to be sent: ");
    Serial.println(voltage);
     
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(20000);
  }

  String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug)
  {
    //Serial.print(response);
  }
  return response;
  
}
