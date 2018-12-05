#ifndef _NETWORK_H_
#define _NETWORK_H_

// Select modem model:

#include <Arduino.h>
#include <ArduinoJson.h>

// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>

#define SIM800L_SOFTWARESERIAL_PIN_RX	2
#define SIM800L_SOFTWARESERIAL_PIN_TX	3

class Network  
{
	public:
	
		// Server details
		const char server[] = "www.sundaug.com/";
		const char resource[] = "";
		const int  port = 80;

		// Consturcutor
	    Network();
		
		// Connect to server
		void connect(String url);

		void init();

		char getServer();

		char getPort();

		String getResponse();
   
    void parseJSONResponse(const char *buffer, unsigned int bufferSize, char *response);

    int readBuffer(char *buffer, int count, unsigned int timeOut);
    
    void cleanBuffer(char *buffer, int count);
	private:
		// Leave empty, if missing user or pass
		const char apn[]  = "sunda";
		const char user[] = "";
		const char pass[] = "";
    DynamicJsonBuffer responseBuffer;
    JsonObject& responseRoot;

		//Post data formats
		String contentType = "application/x-www-form-urlencoded";

		String mResponse;

		void setResponse(String response);
	
};

#endif // _NETWORK_H_
 
