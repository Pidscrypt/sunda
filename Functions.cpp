#include "Functions.h"
#include <Arduino.h>
#include "Switchable.h"
#include "Colours.h"

Switchable red_led(LED_RED);
Switchable blue_led(LED_BLUE);
Switchable orange_led(LED_ORANGE);
Colours colours;

Functions::Functions(){
    
}

//turn on:
void Functions::turnOn(int colour)
{
    switch (colour)
    {
        case COLOR_RED:
            if(!red_led.getState()){
                red_led.on();
                blue_led.off();
                orange_led.off();
            }
            break;
        case COLOR_ORANGE:
            if(!orange_led.getState()){
                orange_led.on();
                blue_led.off();
                red_led.off();
            }
            break;
        case COLOR_BLUE:
            if(!blue_led.getState()){
                blue_led.on();
                red_led.off();
                orange_led.off();
            }
            break;
        default:
            break;
    }
}

void Functions::turnOffAll(){
    if(red_led.getState()){
        red_led.off();
    }
    if(orange_led.getState()){
        orange_led.off();
    }
    if(blue_led.getState()){
        blue_led.off();
    }
}
