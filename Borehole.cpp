#include "Borehole.h"
#include "Arduino.h"
#include "SolenoidValve.h"


SolenoidValve solenoidValve;

Borehole::Borehole(){
  
        pulseCount        = 0;
        flowRate          = 0.0;
        flowMilliLitres   = 0;
        totalMilliLitres  = 0;
        oldTime           = 0;
}

void Borehole::close(){
    Serial.println(F("Closing valve"));
    // logic to close Borehole
    solenoidValve.closeValve();
    mState = false;
}

void Borehole::open(){
    Serial.println(F("Openning valve"));
    // logic to close Borehole
    solenoidValve.openValve();
    mState = true;
}

boolean Borehole::isOpen(){
    return mState;
}

boolean Borehole::getState(){
    return mState;
}

float Borehole::setUsedVolume(float usedVolume){
    waterDrawn = usedVolume;
}

float Borehole::getUsedVolume(){
    return waterDrawn;
}

float Borehole::setMaxWaterToDraw(const float maxVolume){
    maxWaterToDraw = maxVolume;
}

float Borehole::getMaxWaterVolumeToTake(){
    return maxWaterToDraw;
}


/*
Insterrupt Service Routine
 */
void Borehole::pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
