#include "Colours.h"

Colours::Colours(){
    setColorBlue(COLOR_BLUE);
    setColorOrange(COLOR_ORANGE);
    setColorRed(COLOR_RED);
}

int Colours::getBlue(){
    return mColourBlue;
}

int Colours::getOrange(){
    return mColourOrange;
}

int Colours::getRed(){
    return mColourRed;
}

void Colours::setColorBlue(int blue){
    mColourBlue = blue;
}

void Colours::setColorOrange(int orange){
    mColourOrange = orange;
}

void Colours::setColorRed(int red){
    mColourRed = red;
}