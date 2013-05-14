/* Library for reading values from Parallax's Mouse Sensor Kit (#28560)
   Translated from the pBasic code provided by parallax for their Basic Stamp
   Created by Michael Backus, November 7, 2012
   Released into the public domain.
   
   Assumes the Advanced Three-wire Connection
   Does not work with the Quick Start Circuit
*/

#ifndef ParallaxMouseSensor_h
#define ParallaxMouseSensor_h

#include "Arduino.h"

class ParallaxMouseSensor
{
  public:
    ParallaxMouseSensor();
    void configure(byte clock, byte data);
    void readSensor();
    int dx;
    int x;
    int dy;
    int y;
    byte quality;
    byte ovfl;
  private:
    void WriteAddr(byte address, byte data);
    int ReadAddr(byte address);    
    byte sclk;
    byte sdio;
    byte stat;
    byte readx;
    byte ready;
    
};

#endif