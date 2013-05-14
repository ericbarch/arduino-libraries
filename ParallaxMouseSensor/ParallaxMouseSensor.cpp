/* Library for reading values from Parallax's Mouse Sensor Kit (#28560)
   Translated from the pBasic code provided by parallax for their Basic Stamp
   Created by Michael Backus, November 7, 2012
   Released into the public domain.
   
   Assumes the Advanced Three-wire Connection
   Does not work with the Quick Start Circuit
*/

#include <ParallaxMouseSensor.h>

//Mouse sensor register addresses
#define CONF 0x1B
#define LORES 0x80
#define CHNG  0x80
#define OFLOW 0x18
#define NEG 0x80

byte sclk;
byte sdio;
byte stat;
byte quality;
byte ovfl;
byte readx;
byte ready;

int dx;
int dy;
int x;
int y;

ParallaxMouseSensor::ParallaxMouseSensor()
{}

void ParallaxMouseSensor::configure(byte clock, byte data)
{
  sclk = clock;
  sdio = data;
  pinMode(sclk,OUTPUT);
  digitalWrite(sclk,LOW);  
  delay(100);
  WriteAddr(CONF,LORES);
}

void ParallaxMouseSensor::WriteAddr(byte address, byte data)
{
  pinMode(sdio,OUTPUT);
  shiftOut(sdio,sclk,MSBFIRST,address|0x80);
  shiftOut(sdio,sclk,MSBFIRST,data);
}

int ParallaxMouseSensor::ReadAddr(byte address)
{
  int data;
  pinMode(sdio,OUTPUT);
  shiftOut(sdio,sclk,MSBFIRST,address);
  delayMicroseconds(100);
  pinMode(sdio,INPUT);
  data = shiftIn(sdio,sclk,MSBFIRST);
  return data;
}

void ParallaxMouseSensor::readSensor()
{
  stat = ReadAddr(0x16);
  if(stat & CHNG == 0)
  {
  } else {
    if(stat & OFLOW)
    {
      ovfl = 10;
    }
    readx = ReadAddr(0x03);
    if(readx & NEG)
    {
      dx = -1*(256-readx);
    } else {
      dx = readx; 
    }
    x = x + dx;
    ready = ReadAddr(0x02);
    if(ready & NEG)
    {
      dy = -1*(256-ready);
    } else {
      dy = ready;
    }
    y = y + dy;
    quality = ReadAddr(0x04);
    if(ovfl > 0)
    {
      ovfl = ovfl - 1;
    }
  }
}