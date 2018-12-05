#ifndef _BOREHOLE_H_
#define _BOREHOLE_H_

#include <Arduino.h>


#define SOLENOIDVALVE_PIN_COIL1	4

class Borehole{
    
    public:
        // Constructor
        Borehole();

        // let water flow
        void open();

        // stop water flow
        void close();

        // get ammount of water taken
        float getUsedVolume();

        float setUsedVolume(float usedVolume);

        float setMaxWaterToDraw(const float maxVolume);

        float getMaxWaterVolumeToTake();

        boolean isOpen();

        void pulseCounter();
    
    private:
        long maxWaterToDraw = 0;

        boolean mState;

        long waterDrawn;

        boolean getState();
        
        // The hall-effect flow sensor outputs approximately 4.5 pulses per second per
        // litre/minute of flow.
        float calibrationFactor = 4.5;
        
        volatile byte pulseCount;  
        
        float flowRate;
        unsigned int flowMilliLitres;
        unsigned long totalMilliLitres;
        unsigned long totalLitres;
        
        unsigned long oldTime;
};

#endif //_BOREHOLE_H_
