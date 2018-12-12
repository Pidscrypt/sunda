/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>

File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  addTag("4653427235475743");

  // re-open the file for reading:
  myFile = SD.open("hello.csv");
  if (myFile) {
    //Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      //Serial.write(myFile.read());
      //String str  = myFile.readStringUntil('\n');
      String str = (String)myFile.readStringUntil('\n');
      Serial.println(str);
      /*if(getBalance(str)){
        Serial.println(str);
        break;
      }*/
      
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

boolean getBalance(String str){
  return true;
}

boolean addTag(String rfid){
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("hello.csv", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {

    boolean found = false;
    
    //while(myFile.available()){
//      String check = myFile.readStringUntil('\n');
      String check = "90757680977765,43re,32424";
      char chk[200];

      struct sub{
        char tag_id[20];
        char bal[10];
        char vol[10];
        };

        sub client;
        char temp_id[20];
      strcpy(chk,check.c_str());
      String oldId = "";
      
      for(int i = 0; i < ((sizeof(chk)/sizeof(*chk)) - 1); i++){
        Serial.print(chk[i]);
        strcat(temp_id,check[i]);
        if(check[i] == ','){
          Serial.print(temp_id);
          break;
        }

     
        oldId = oldId + check[i];
      }
      Serial.println("\n");
      /*if(oldId == rfid){
        found = true;
        Serial.println(check);
        break;
      }*/
    //}

    /*if(!found){
      Serial.print("RFID does not exists.. so we add new one");
      myFile.print(rfid);
      myFile.print(",");
      myFile.print("45433,");
      myFile.print("2757\n");
    }*/
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  // nothing happens after setup
}
