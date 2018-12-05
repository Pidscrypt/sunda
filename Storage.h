#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <Arduino.h>
#include <SdFat.h>

//SdFat sd;
//==============================================================================
// Error messages stored in flash.
#define error(msg) sd.errorHalt(F(msg))

//#define SDFILE_PIN_CS	10
//#define SDFILE_PIN_CS SS

class Storage {
    public:
        // Conrtactor
        Storage(int chipSelectPin);

        // initialize SD card
        void initialise();

        void updateBalance(String id, float balance);

        void writeHeader();

        void finish();
        
    private:
                
        // SD chip select pin.  Be sure to disable any other SPI devices such as Enet.
        const uint8_t chipSelect = SS;

        // Interval between data records in milliseconds.
        // The interval must be greater than the maximum SD write latency plus the
        // time to acquire and write data to the SD to avoid overrun errors.
        // Run the bench example to check the quality of your SD card.
        const uint32_t SAMPLE_INTERVAL_MS = 1000;

        // Log file base name.  Must be six characters or less.
        #define FILE_BASE_NAME "sunda"
        //------------------------------------------------------------------------------
        // File system object.
//        SdFat sd;

        // Log file.
        SdFile file;

        // Time in micros for next data record.
        uint32_t logTime;

        //==============================================================================
        // User functions.  Edit writeHeader() and logData() for your requirements.

        const uint8_t ANALOG_COUNT = 2;
        int SDFILE_PIN_CS;
};

#endif // Storage
