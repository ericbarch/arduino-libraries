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

#include <stdlib.h>
#include <string.h>
#include "Arduino.h"
#include "Wire.h"
#include "Relay16.h"

Relay16::Relay16()
{
	this->addr = 0;
	this->relayBits = 0; // All relays off
	this->dioReset = -1; // Unknown digital I/O for the reset line
}

void Relay16::refresh()
{
   Wire.beginTransmission(this->addr);
   Wire.write(~(this->relayBits & 0xFF));
   Wire.write(~((this->relayBits >> 8) & 0xFF));
   Wire.endTransmission();
}


void Relay16::allOff()
{
	this->relayBits = 0;
	refresh();
}

void Relay16::allOn()
{
	this->relayBits = 0xFFFF;
	refresh();
}

void Relay16::relayOn(byte relayNum)
{
	if (relayNum < 1 || relayNum > 16)
		return;
	this->relayBits |= 1<<(relayNum-1);
	refresh();
}

void Relay16::relayOff(byte relayNum)
{
	if (relayNum < 1 || relayNum > 16)
		return;
	this->relayBits &= ~(1<<(relayNum-1));
	refresh();
}


void Relay16::begin(boolean j5, boolean j6, boolean j7, char dioResetPin)
{
	uint8_t addrBitmap = (j5?0x01:0x00) | (j6?0x02:0x00) | (j7?0x04:0x00);
	this->addr = 0x20 | (addrBitmap);
	
	// If there's a DIO pin assigned to reset, use it to do a hardware reset on initialization
	if (-1 != dioResetPin)
	{
		pinMode(dioResetPin, OUTPUT);
		digitalWrite(dioResetPin, LOW);
		delayMicroseconds(100);
		digitalWrite(dioResetPin, HIGH);
	}
	
	this->allOff();
}

