#include <Arduino.h>
#include "SolenoidValve.h"

SolenoidValve::SolenoidValve(){
    pinMode(valveSelect, OUTPUT);
    pinMode(RELAY_ONE, OUTPUT);
    pinMode(RELAY_TWO, OUTPUT);
}

void SolenoidValve::openValve(){
    digitalWrite(valveSelect, HIGH);
    digitalWrite(RELAY_ONE, LOW);
    digitalWrite(RELAY_TWO, LOW);
    delay(15000);
    finish();
}

void SolenoidValve::closeValve(){
    digitalWrite(valveSelect, HIGH);
    digitalWrite(RELAY_ONE, HIGH);
    digitalWrite(RELAY_TWO, LOW);
    delay(15000);
    finish();
}

void SolenoidValve::finish(){
    digitalWrite(valveSelect, LOW);
}
