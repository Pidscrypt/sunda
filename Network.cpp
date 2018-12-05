#include "Network.h"
#include <Arduino.h>
#include <string.h>
#include <ArduinoJson.h>

// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>

SoftwareSerial SIM800L(SIM800L_SOFTWARESERIAL_PIN_RX,SIM800L_SOFTWARESERIAL_PIN_TX);

Network::Network(){
    
}

void Network::init(){  
    //Serial.begin(9600); 
    SIM800L.begin(9600);
}

void Network::connect(String url){
  // put your main code here, to run repeatedly:
  SIM800L.println("AT+CGATT?");
  delay(100);
  while(SIM800L.available())
  {
    Serial.write(SIM800L.read());
  }

  SIM800L.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n");
  delay(100);
  while(SIM800L.available())
  {
    Serial.write(SIM800L.read());
  }

  SIM800L.println("AT+SAPBR=3,1,\"APN\",\"3gprs\"");
  delay(100);
  while(SIM800L.available())
  {
    Serial.write(SIM800L.read());
  }

  SIM800L.println("AT+SAPBR=1,1\r\n");
  delay(1500);
  while(SIM800L.available())
  {
    Serial.write(SIM800L.read());
  }

  SIM800L.println("AT+HTTPINIT\r\n");
  delay(1500);
  while(SIM800L.available())
  {
    Serial.write(SIM800L.read());
  }

  SIM800L.print("AT+HTTPPARA=\"URL\",\"http://sundaug.com/");
  SIM800L.print(url);
  SIM800L.println("\"");
  delay(2000);
  while(SIM800L.available())
  {
    Serial.write(SIM800L.read());
  }

  SIM800L.println("AT+HTTPACTION=0");
  delay(4000);
  while(SIM800L.available())
  {
    Serial.write(SIM800L.read());
  }
  SIM800L.flush();

  String res;
  char in;

  SIM800L.println("AT+HTTPREAD");
  delay(2000);
  while(SIM800L.available())
  {
    //Serial.write(SIM800L.read());
    in = SIM800L.read();
    Serial.write(in);
    res += in;
    //Serial.println(res);
  }

  if(res != ""){
    Serial.println("My data here");
    Serial.println(res);
    setResponse(res);
  }else{
    Serial.println("No data returned");
  }

  Serial.println("");
  SIM800L.println("AT+HTTPTERM");
  delay(1000);
  while(SIM800L.available())
  {
    Serial.write(SIM800L.read());
  }
}


int Network::readBuffer(char *buffer, int count, unsigned int timeOut)
{
    int i = 0;
    unsigned long timerStart,timerEnd;
    timerStart = millis();
    while(1) {
        while (SIM800L.available()) {
            char c = SIM800L.read();
            buffer[i] = c;
            buffer[i + 1] = '\0';
            ++i;
            if(i > count-1)break;
        }
        if(i > count-1)break;
        timerEnd = millis();
        if(timerEnd - timerStart > timeOut) {
            break;
        }
    }

    while(SIM800L.available()) {
        SIM800L.read();
    }
    return true;
}

void Network::cleanBuffer(char *buffer, int count)
{
    for(int i=0; i < count; i++) {
        buffer[i] = '\0';
    }
}

void Network::setResponse(String response){
  mResponse = response;
  /*char buffer[128];
  cleanBuffer(buffer, sizeof(buffer));
  cleanBuffer(response, sizeof(response));

  if (readBuffer(buffer, sizeof(buffer), 1) == true){
    parseJSONResponse(buffer, sizeof(buffer), response);
  }
  Serial.println(response);
    StaticJsonBuffer<32> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(response);
    mResponse = root["households"].asString();*/
    //Serial.write(root["districts"]);
}

String Network::getResponse(){
  return mResponse;
}

char Network::getPort(){
  return port;
}

char Network::getServer(){
  return server;
}

void Network::parseJSONResponse(const char *buffer, unsigned int bufferSize, char *response){

  int start_index = 0;
  int i = 0;
  while (i < bufferSize - 1 && start_index == 0) {
    char c = buffer[i];
    if ('{' == c){
      start_index = i;
    }
    ++i;
  }

  int end_index = 0;
  int j = bufferSize - 1;
  while (j >= 0 && end_index == 0) {
    char c = buffer[j];
    if ('}' == c) {
      end_index = j;
    }
    --j;
  }

  for(int k = 0; k < (end_index - start_index) + 2; ++k){
    response[k] = buffer[start_index + k];
    response[k + 1] = '\0';
  }
}
