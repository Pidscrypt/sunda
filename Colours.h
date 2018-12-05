#ifndef _COLOURS_H_
#define _COLOURS_H_

#define COLOR_RED 0
#define COLOR_BLUE 1
#define COLOR_ORANGE 2

class Colours {

    public:

        Colours(); 

        int getRed();
        int getBlue();
        int getOrange();

        void setColorRed(int red);
        void setColorBlue(int blue);
        void setColorOrange(int orange);

    private:
        int mColourRed;
        int mColourOrange;
        int mColourBlue;
};

#endif