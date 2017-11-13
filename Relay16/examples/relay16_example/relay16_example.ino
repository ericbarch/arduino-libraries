/*************************************************************************
Title:    Iowa Scaled Engineering I2C-RELAY16 Driver Library Example Sketch
Authors:  Nathan D. Holmes <maverick@drgw.net>
File:     $Id: $
License:  GNU General Public License v3

LICENSE:
    Copyright (C) 2014 Nathan D. Holmes & Michael D. Petersen

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    For more information about the Iowa Scaled Engineering I2C-RELAY16, see:
    http://www.iascaled.com/store/I2C-RELAY16

*************************************************************************/

#include <Wire.h>
#include <Relay16.h>

Relay16 relayBoard;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  Wire.begin();
//  Both of the following initialization examples assume that address jumpers J5, J6, and J7
//  on the I2C-RELAY16 are set to low (center pin is jumpered to the one away from the + sign on
//  each jumper block).  If they're set the other direction (center to + pin), replace "LOW" with "HIGH"

//  If you're using a standard Iowa Scaled shield to connect the I2C lines to the Arduino,
//  the /IORST line is likely tied to Digital IO line 4.  Use the initializer below.
  relayBoard.begin(LOW, LOW, LOW, 4);

//  If the /IORST line on the I2C cable isn't connected anywhere, use this initializer
//   instead of the one above.
//  relayBoard.begin(LOW, LOW, LOW, -1);

}

void loop() {
  // print the results to the serial monitor:

  byte relayNum;

  for (relayNum = 1; relayNum <= 16; relayNum++)
  {
    relayBoard.relayOn(relayNum);
    delay(250);
    relayBoard.relayOff(relayNum);  
    
  }

  relayBoard.allOn();
  delay(250);
  relayBoard.allOff();
  delay(250);   

}
