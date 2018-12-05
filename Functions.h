#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_


#include <Arduino.h>


#define LED_RED A3
#define LED_ORANGE A4
#define LED_BLUE A5

//Base class for output that can be switched on/off via single digital pin:
class Functions  
{
	public:

		// Consturcutor
	    Functions();
		
		// Turn LED light on
		void turnOn(int colour);

    // Turn all lights off
    void turnOffAll();
	
	private:
	
		int mColour; 	//output pin
};

#endif // _FUNCTIONS_H_
 
