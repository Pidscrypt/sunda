#ifndef _SOLENOID_VALVE_H_
#define _SOLENOID_VALVE_H_

#include "Switchable.h"

#define valveSelect 9
#define RELAY_ONE 8
#define RELAY_TWO 7

//solenoid driver class:
class SolenoidValve {
    public:
        // Constructor
        SolenoidValve();

        void openValve();
        void closeValve();
        void finish();
};

#endif // _SOLENOID_VALVE_H_
 
