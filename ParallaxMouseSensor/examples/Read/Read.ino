/* Library for reading values from Parallax's Mouse Sensor Kit (#28560)
   Translated from the pBasic code provided by parallax for their Basic Stamp
   Created by Michael Backus, November 7, 2012
   Released into the public domain.
   
   Assumes the Advanced Three-wire Connection
   Does not work with the Quick Start Circuit
*/

#include <ParallaxMouseSensor.h>

ParallaxMouseSensor mouse;

void setup()
{
  Serial.begin(9600);
  mouse.configure(5,6);  //sets clock to pin 5 and data to pin 6 
}

void loop()
{
  mouse.readSensor();
  Serial.print(mouse.dx,DEC);
  Serial.print("\t");
  Serial.print(mouse.x,DEC);
  Serial.print("\t");
  Serial.print(mouse.dy,DEC);
  Serial.print("\t");
  Serial.print(mouse.y,DEC);
  Serial.print("\t");
  Serial.print(mouse.quality,DEC);
  if(mouse.ovfl > 0)
    {

      Serial.print("\t");
      Serial.print("Overflow");
    }
  Serial.print("\n");
}
