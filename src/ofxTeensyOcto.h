#pragma once

#include "ofMain.h"

class ofxTeensyOcto
{
public:
    
    // teensy functions
    void setup(int _ledWidth, int _ledHeight, int _stripsPerPort, int _numPorts);
    void update();
    void serialConfigure(string portName, float _xoffset, float _yoffset, float _widthPct, float _heightPct, int _direction);
    void image2data(ofImage image, unsigned char* data, bool layout);
    void draw(int x, int y);

    // arrays
    ofSerial * ledSerial;
    ofRectangle * ledArea;
    bool * ledLayout;           // layout of rows, true = even is left->right
    ofImage * ledImage;         // image sent to each port
    ofColor * colors;
    unsigned char *ledData;
    
    // variables
    ofPixels pixels1;
    ofPixels pixels2;
    int ledWidth;
    int ledHeight;
    int stripsPerPort;
    int numPortsMain;
    int numPorts;
    int maxPorts;
    int dataSize;
    bool simulate;

    // demos
    float waveSpeed;
    int brightness;
    float counterShape;
    int hue;
    

    // translate the 24 bit color from RGB to the actual
    // order used by the LED wiring.  GRB is the most common.
    inline int colorWiring(int c)
    {
        // GRB wiring
        return ((c & 0xFF0000) >> 8) | ((c & 0x00FF00) << 8) | (c & 0x0000FF);
    }
    
    // convert an integer from 0 to 100 to a float percentage
    // from 0.0 to 1.0.  Special cases for 1/3, 1/6, 1/7, etc
    // are handled automatically to fix integer rounding.
    inline double percentageFloat(float percent)
    {
        if (percent == 33) return 1.0 / 3.0;
        if (percent == 17) return 1.0 / 6.0;
        if (percent == 14) return 1.0 / 7.0;
        if (percent == 13) return 1.0 / 8.0;
        if (percent == 11) return 1.0 / 9.0;
        if (percent ==  9) return 1.0 / 11.0;
        if (percent ==  8) return 1.0 / 12.0;
        return (double)percent / 100.0;
    }
    
    // scale a number by a percentage, from 0 to 100
    inline float percentage(float num, float percent)
    {
        double mult = percentageFloat(percent);
        double output = num * mult;
        return (float)output;
    }
    
    // scale a number by the inverse of a percentage, from 0 to 100
    inline int percentageInverse(int num, int percent)
    {
        double div = percentageFloat(percent);
        double output = num / div;
        return (int)output;
    }
};
