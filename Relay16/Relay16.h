/*************************************************************************
Title:    Iowa Scaled Engineering I2C-RELAY16 Driver Library
Authors:  Nathan D. Holmes <maverick@drgw.net>
File:     $Id: $
License:  GNU General Public License v3

LICENSE:
    Copyright (C) 2014 Nathan D. Holmes & Michael D. Petersen

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    For more information about the Iowa Scaled Engineering I2C-RELAY16, see:
    http://www.iascaled.com/store/I2C-RELAY16
    

*************************************************************************/

#ifndef RELAY16_H
#define RELAY16_H

#include "Arduino.h"

class Relay16
{
	public:
		Relay16();
		void begin(boolean j5, boolean j6, boolean j7, char dioReset);
		void relayOn(byte relayNum);
		void relayOff(byte relayNum);
		void allOff();
		void allOn();
		void refresh();
	private:
		uint16_t relayBits;
		uint8_t addr;
		int8_t dioReset;
};


#endif

