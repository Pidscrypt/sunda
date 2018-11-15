/*
o  o o--o   O  o-o    o-o      o   o o--o  
|  | |     / \ |  \  |         |   | |   | 
O--O O-o  o---o|   O  o-o      |   | O--o  
|  | |    |   ||  /      |     |   | |     
o  o o--o o   oo-o   o--o       o-o  o     
                                           
1. If you have a new ATmega328P , youâ€™ll need to burn the bootloader onto the chip first.
Please follow our instructions in the following tutorial:
https://www.circuito.io/blog/atmega328p-bootloader/                                      

2. After uploading the sketches onto the ATMega328P, open the serial monitor and reboot the ATMega328P to view the test code menu.
*/
// Include Libraries
#include "Arduino.h"
#include "RFID.h"
#include "SD.h"
#include "SolenoidValve.h"


// Pin Definitions
#define RFID_PIN_SDA	6
#define RFID_PIN_RST	5
#define SDFILE_PIN_CS	10
#define SIM800L_SOFTWARESERIAL_PIN_RX	3
#define SIM800L_SOFTWARESERIAL_PIN_TX	2
#define SOLENOIDVALVE_PIN_COIL1	4
#define WATERFLOW5V_PIN_SIG	A0



// Global variables and defines

// object initialization
File sdFile;
RFID rfid(RFID_PIN_SDA,RFID_PIN_RST);
SolenoidValve solenoidValve(SOLENOIDVALVE_PIN_COIL1);



// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    //initialize RFID module
    rfid.init();
    // Set SPI SS pin to output otherwise the SD library functions will not work.
    // The SD is set to use SPI SS Arduino pin 10 as chip select(CS) by default.
    // To change the pin use SD.begin(SD_CS_PIN)
    pinMode(SDFILE_PIN_CS, OUTPUT);
    // Check if the card is present and can be initialized
    if (!SD.begin()) {
        Serial.println(F("Card failed, or not present"));
        while(1);
    }
    Serial.println(F("card initialized."));
    menuOption = menu();
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    
    
    if(menuOption == '1') {
    // RFID - RC522 RF IC Card Sensor Module - Test Code
    //Read RFID tag if present
    String rfidtag = rfid.readTag();
    //print the tag to serial monitor if one was discovered
    rfid.printTag(rfidtag);

    }
    else if(menuOption == '2') {
    // Micro SD Card Memory Shield Module - Test Code
    // The SD example code creates a datalog.txt file for logging sensor data
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    sdFile = SD.open("datalog.txt", FILE_WRITE);
    // if the file exists in SD card, write sensor data
    if (sdFile) {
        //Write to file
        sdFile.println("ENTER SENSOR DATA HERE");
        // close the file
        sdFile.close();
        // Uncomment to print to the serial port
        //Serial.println("ENTER SENSOR DATA HERE");
    } 
    else {
        // if the file didn't open, print an error
        Serial.println(F("error opening file."));
    }
    }
    else if(menuOption == '3') {}
    // Disclaimer: The QuadBand GPRS-GSM SIM800L is in testing and/or doesn't have code, therefore it may be buggy. Please be kind and report any bugs you may find.
    else if(menuOption == '4') {
    // 12V Solenoid Valve - 3/4'' - Test Code
    // The solenoid valve will turn on and off for 500ms (0.5 sec)
    solenoidValve.on(); // 1. turns on
    delay(500);       // 2. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.
    solenoidValve.off();// 3. turns off
    delay(500);       // 4. waits 500 milliseconds (0.5 sec). Change the value in the brackets (500) for a longer or shorter delay in milliseconds.

    }
    else if(menuOption == '5') {}
    // Disclaimer: The Water Flow Sensor G1/2'' is in testing and/or doesn't have code, therefore it may be buggy. Please be kind and report any bugs you may find.
    
    
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    
}



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) RFID - RC522 RF IC Card Sensor Module"));
    Serial.println(F("(2) Micro SD Card Memory Shield Module"));
    Serial.println(F("(3) QuadBand GPRS-GSM SIM800L"));
    Serial.println(F("(4) 12V Solenoid Valve - 3/4''"));
    Serial.println(F("(5) Water Flow Sensor G1/2''"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {
            if(c == '1') 
    			Serial.println(F("Now Testing RFID - RC522 RF IC Card Sensor Module"));
    		else if(c == '2') 
    			Serial.println(F("Now Testing Micro SD Card Memory Shield Module"));
    		else if(c == '3') 
    			Serial.println(F("Now Testing QuadBand GPRS-GSM SIM800L - note that this component doesn't have a test code"));
    		else if(c == '4') 
    			Serial.println(F("Now Testing 12V Solenoid Valve - 3/4''"));
    		else if(c == '5') 
    			Serial.println(F("Now Testing Water Flow Sensor G1/2''"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
        }
    }
}

/*******************************************************

*    Circuito.io is an automatic generator of schematics and code for off
*    the shelf hardware combinations.

*    Copyright (C) 2016 Roboplan Technologies Ltd.

*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.

*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.

*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*    In addition, and without limitation, to the disclaimers of warranties 
*    stated above and in the GNU General Public License version 3 (or any 
*    later version), Roboplan Technologies Ltd. ("Roboplan") offers this 
*    program subject to the following warranty disclaimers and by using 
*    this program you acknowledge and agree to the following:
*    THIS PROGRAM IS PROVIDED ON AN "AS IS" AND "AS AVAILABLE" BASIS, AND 
*    WITHOUT WARRANTIES OF ANY KIND EITHER EXPRESS OR IMPLIED.  ROBOPLAN 
*    HEREBY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT 
*    NOT LIMITED TO IMPLIED WARRANTIES OF MERCHANTABILITY, TITLE, FITNESS 
*    FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND THOSE ARISING BY 
*    STATUTE OR FROM A COURSE OF DEALING OR USAGE OF TRADE. 
*    YOUR RELIANCE ON, OR USE OF THIS PROGRAM IS AT YOUR SOLE RISK.
*    ROBOPLAN DOES NOT GUARANTEE THAT THE PROGRAM WILL BE FREE OF, OR NOT 
*    SUSCEPTIBLE TO, BUGS, SECURITY BREACHES, OR VIRUSES. ROBOPLAN DOES 
*    NOT WARRANT THAT YOUR USE OF THE PROGRAM, INCLUDING PURSUANT TO 
*    SCHEMATICS, INSTRUCTIONS OR RECOMMENDATIONS OF ROBOPLAN, WILL BE SAFE 
*    FOR PERSONAL USE OR FOR PRODUCTION OR COMMERCIAL USE, WILL NOT 
*    VIOLATE ANY THIRD PARTY RIGHTS, WILL PROVIDE THE INTENDED OR DESIRED
*    RESULTS, OR OPERATE AS YOU INTENDED OR AS MAY BE INDICATED BY ROBOPLAN. 
*    YOU HEREBY WAIVE, AGREE NOT TO ASSERT AGAINST, AND RELEASE ROBOPLAN, 
*    ITS LICENSORS AND AFFILIATES FROM, ANY CLAIMS IN CONNECTION WITH ANY OF 
*    THE ABOVE. 
********************************************************/
