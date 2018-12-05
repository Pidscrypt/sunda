#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
#include "Storage.h"

        SdFat sd;

Storage::Storage(int chipSelectPin){
  //cardSize = 4;
  //pinMode(SDFILE_PIN_CS, OUTPUT);
    

  SDFILE_PIN_CS = chipSelectPin;
  pinMode(6, OUTPUT);
  digitalWrite(6,HIGH);
  pinMode(SDFILE_PIN_CS,OUTPUT);     // Set digital as OUTPUT to connect it to the SD /ENABLE pin 
  digitalWrite(SDFILE_PIN_CS, LOW); 
}
//------------------------------------------------------------------------------------
void Storage::initialise(){
  digitalWrite(SDFILE_PIN_CS, LOW);
    const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
  char fileName[13] = FILE_BASE_NAME ".csv";
  Serial.println(F("Innitialising SD card"));
  
  // Wait for USB Serial 
  /*while (!Serial) {
    SysCall::yield();
  }
  delay(1000);*/
  
  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!sd.begin(SDFILE_PIN_CS, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }

  // Find an unused file name.
  if (BASE_NAME_SIZE > 10) {
    error("FILE_BASE_NAME too long");
  }

  if(sd.exists(fileName)){
    Serial.print(fileName);
    Serial.print(" Alreay exists.");
    Serial.println("\n");
  }else{
    Serial.println("Creating new file");
  }
  /*while (sd.exists(fileName)) {

     if (fileName[BASE_NAME_SIZE] != '0') {
      //fileName[BASE_NAME_SIZE + 1] = '0';
      //fileName[BASE_NAME_SIZE]++;
    } else {
      error("Can't create file name");
    }
  }*/
  if (!file.open(fileName, O_RDWR | O_CREAT | O_AT_END)) {
    error("file.open");
  }

  /*if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) {
    error("file.open");
  }*/
  // Read any Serial data.
  do {
    Serial.println("Reading any Serial data.");
    delay(10);
  } while (Serial.available() && Serial.read() >= 0);

  Serial.print(F("Ready to Log to: "));
  Serial.println(fileName);
  //Serial.println(F("Type any character to stop"));

  // Write data header.
  writeHeader();

  // Start on a multiple of the sample interval.
  logTime = micros()/(1000UL*SAMPLE_INTERVAL_MS) + 1;
  logTime *= 1000UL*SAMPLE_INTERVAL_MS;
  finish();
}

//------------------------------------------------------------------------------
// Write data header.
void Storage::writeHeader() {
  digitalWrite(SDFILE_PIN_CS, LOW);
  Serial.println(F("Creating headers"));
  file.print(F("tagId,"));
  file.print(F("balance,"));
  file.print(F("max_fetch"));
  /*for (uint8_t i = 0; i < ANALOG_COUNT; i++) {
    file.print(F(",adc"));
    file.print(i, DEC);
  }*/
  file.println();
  file.close();
  digitalWrite(SDFILE_PIN_CS, HIGH);
}
//------------------------------------------------------------------------------
// Log a data record.
void Storage::updateBalance(String id, float balance) {
  digitalWrite(SDFILE_PIN_CS, LOW);
  uint16_t data[ANALOG_COUNT];
  const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
  char fileName[13] = FILE_BASE_NAME ".csv";

  // Read all channels to avoid SD write latency between readings.
  for (uint8_t i = 0; i < ANALOG_COUNT; i++) {
    data[i] = analogRead(i);
  }
  // Write data to file.  Start with log time in micros.
  //file.print(logTime);
  if (!file.open(fileName, O_RDWR | O_CREAT | O_AT_END)) {
    error("could not open file here");
  }
  file.print(id);
  file.write(',');
  file.print(balance);
  file.write(',');
  file.print('23');

  // Write ADC data to CSV record.
  /*for (uint8_t i = 0; i < ANALOG_COUNT; i++) {
    file.write(',');
    file.print(data[i]);
  }*/
  file.println();
  file.close();
  finish();
}

void Storage::finish(){
  
  // Force data to SD and update the directory entry to avoid data loss.
  if (!file.sync() || file.getWriteError()) {
    error("write error");
  }

  if (Serial.available()) {
    // Close file and stop.
    file.close();
    Serial.println(F("Done"));
    SysCall::halt();
  }
  //pinMode(SDFILE_PIN_CS, INPUT);

    Serial.println(F("Switching back to RFID"));

    pinMode(SDFILE_PIN_CS, OUTPUT);
    digitalWrite(SDFILE_PIN_CS, HIGH);
    digitalWrite(10, LOW);
}
