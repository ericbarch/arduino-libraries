/*
  nmea.cpp - NMEA 0183 sentence decoding library for Wiring & Arduino
  Copyright (c) 2008 Maarten Lamers, The Netherlands.
  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Arduino.h"
#include "nmea.h"

#define _GPRMC_TERM   "$GPRMC,"		// GPRMC datatype identifier
#define _LIB_VERSION	1						// software version of this library

//
// constructor method
//

NMEA::NMEA(int connect)
{
	// private properties
	_gprmc_only = connect;
	_gprmc_utc = 0.0;
	_gprmc_status = 'V';
	_gprmc_lat = 0.0;
	_gprmc_long = 0.0;
	_gprmc_speed = 0.0;
	_gprmc_angle = 0.0;
	_terms = 0;
	n = 0;
	_state = 0;
	_parity = 0;
	_nt = 0;

	f_sentence[0] = 0;
	f_terms = 0;
	// allocate memory for individual terms of sentence
  for (int t=0; t<30; t++) {
    _term[t] = (char*) malloc (15 * sizeof(char));
    f_term[t] = (char*) malloc (15 * sizeof(char));
    (f_term[t])[0] = 0;
  }
}


//
// public methods
//

int NMEA::decode(char c) {
  // avoid runaway sentences (>99 chars or >29 terms) and terms (>14 chars)
  if ((n >= 100) || (_terms >= 30) || (_nt >= 15)) { _state = 0; }
  // LF and CR always reset parser
  if ((c == 0x0A) || (c == 0x0D)) { _state = 0; }
  // '$' always starts a new sentence
  if (c == '$') {
    _gprmc_tag = 0;
    _parity = 0;
    _terms = 0;
    _nt = 0;
    _sentence[0] = c;
    n = 1;
    _state = 1;
    return 0;
  }
  // parse other chars according to parser state
  switch(_state) {
  case 0:
    // waiting for '$', do nothing
    break;
  case 1:
  	// decode chars after '$' and before '*' found
    if (n < 7) {
    	// see if first seven chars match "$GPRMC,"
    	if (c == _GPRMC_TERM[n]) { _gprmc_tag++; }
    }
    // add received char to sentence
    _sentence[n++] = c;
    switch (c) {
    case ',':
    	// ',' delimits the individual terms
      (_term[_terms++])[_nt] = 0;
      _nt = 0;
      _parity = _parity ^ c;
      break;
    case '*':
    	// '*' delimits term and precedes checksum term
      (_term[_terms++])[_nt] = 0;
      _nt = 0;
      _state++;
      break;
    default:
    	// all other chars between '$' and '*' are part of a term
      (_term[_terms])[_nt++] = c;
      _parity = _parity ^ c;
      break;
    }
    break;
  case 2:
  	// first char following '*' is checksum MSB
    _sentence[n++] = c;
    (_term[_terms])[_nt++] = c;
    _parity = _parity - (16 * _dehex(c));		// replace with bitshift?
    _state++;
    break;
  case 3:
  	// second char after '*' completes the checksum (LSB)
    _sentence[n++] = c;
    _sentence[n++] = 0;
    (_term[_terms])[_nt++] = c;
    (_term[_terms++])[_nt] = 0;
    _state = 0;
    _parity = _parity - _dehex(c);
    // when parity is zero, checksum was correct!
    if (_parity == 0) {
    	// accept all sentences, or only GPRMC datatype?
    	if ((!_gprmc_only) || (_gprmc_tag == 6)) {
	    	// copy _sentence[] to f_sentence[]
	    	while ((--n) >= 0) { f_sentence[n] = _sentence[n]; }
	    	// copy all _terms[] to f_terms[]
	      for (f_terms=0; f_terms<_terms; f_terms++) {
	      	_nt = 0;
	      	while ((_term[f_terms])[_nt]) {
	        	(f_term[f_terms])[_nt] = (_term[f_terms])[_nt];
	        	_nt++;
	        }
	        (f_term[f_terms])[_nt] = 0;
	      }
	      // when sentence is of datatype GPRMC
	      if (_gprmc_tag == 6) {
	      	// store values of relevant GPRMC terms
	      	_gprmc_utc = _decimal(_term[1]);
	      	_gprmc_status = (_term[2])[0];
	        // calculate signed degree-decimal value of latitude term
	        _gprmc_lat = _decimal(_term[3]) / 100.0;
	        _degs = floor(_gprmc_lat);
	        _gprmc_lat = (100.0 * (_gprmc_lat - _degs)) / 60.0;
	        _gprmc_lat += _degs;
	        // southern hemisphere is negative-valued
	        if ((_term[4])[0] == 'S') {
	          _gprmc_lat = 0.0 - _gprmc_lat;
	        }
	        // calculate signed degree-decimal value of longitude term
	        _gprmc_long = _decimal(_term[5]) / 100.0;
	        _degs = floor(_gprmc_long);
	        _gprmc_long = (100.0 * (_gprmc_long - _degs)) / 60.0;
	        _gprmc_long += _degs;
	        // western hemisphere is negative-valued
	        if ((_term[6])[0] == 'W') {
	          _gprmc_long = 0.0 - _gprmc_long;
	        }
	        _gprmc_speed = _decimal(_term[7]);
	        _gprmc_angle = _decimal(_term[8]);
	      }
      	// sentence accepted!
      	return 1;
      }
    }
    break;
  default:
    _state = 0;
    break;
  }
  return 0;
}

float NMEA::gprmc_utc() {
	// returns decimal value of UTC term of last-known GPRMC sentence
	return _gprmc_utc;
}

char NMEA::gprmc_status() {
	// returns status character of last-known GPRMC sentence ('A' or 'V')
	return _gprmc_status;
}

float NMEA::gprmc_latitude() {
	// returns signed degree-decimal latitude value of last-known GPRMC position
	return _gprmc_lat;
}

float NMEA::gprmc_longitude() {
	// returns signed degree-decimal longitude value of last-known GPRMC position
	return _gprmc_long;
}

float NMEA::gprmc_speed(float unit) {
	// returns speed-over-ground from last-known GPRMC sentence
	return (_gprmc_speed * unit);
}

float NMEA::gprmc_course() {
	// returns decimal value of track-angle-made-good term in last-known GPRMC sentence
	return _gprmc_angle;
}

float NMEA::gprmc_distance_to(float latitude, float longitude, float unit) {
	// returns distance from last-known GPRMC position to given position
	return distance_between( _gprmc_lat, _gprmc_long, latitude, longitude, unit);
}

float NMEA::gprmc_course_to(float latitude, float longitude) {
	// returns initial course in degrees from last-known GPRMC position to given position
	return initial_course( _gprmc_lat, _gprmc_long, latitude, longitude);
}

//float NMEA::gprmc_rel_course_to(float latitude, float longitude) {
//	// returns course in degrees to given position, relative to last-known GPRMC track angle
//	float rc = initial_course( _gprmc_lat, _gprmc_long, latitude, longitude) - _gprmc_angle;
//	if (rc < 0.0) {
//		rc += 360.0;
//	}
//	return rc;
//}

char* NMEA::sentence() {
	// returns last received full sentence as zero terminated string
  return f_sentence;
}

int NMEA::terms() {
	// returns number of terms (including data type and checksum) in last received full sentence
  return f_terms;
}

char* NMEA::term(int t) {
	// returns term t of last received full sentence as zero terminated string
  return f_term[t];
}

float NMEA::term_decimal(int t) {
	// returns value of decimally coded term t
  return _decimal(f_term[t]);
}

int NMEA::libversion() {
	// returns software version of this library
  return _LIB_VERSION;
}


//
// private methods
//

float NMEA::distance_between (float lat1, float long1, float lat2, float long2, float units_per_meter) {
	// returns distance in meters between two positions, both specified
	// as signed decimal-degrees latitude and longitude. Uses great-circle
	// distance computation for hypothised sphere of radius 6372795 meters.
	// Because Earth is no exact sphere, rounding errors may be upto 0.5%.
  float delta = radians(long1-long2);
  float sdlong = sin(delta);
  float cdlong = cos(delta);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  float slat1 = sin(lat1);
  float clat1 = cos(lat1);
  float slat2 = sin(lat2);
  float clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
  delta = sq(delta);
  delta += sq(clat2 * sdlong);
  delta = sqrt(delta);
  float denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
  delta = atan2(delta, denom);
  return delta * 6372795 * units_per_meter;
}

float NMEA::initial_course (float lat1, float long1, float lat2, float long2) {
	// returns initial course in degrees (North=0, West=270) from
	// position 1 to position 2, both specified as signed decimal-degrees
	// latitude and longitude.
  float dlon = radians(long2-long1);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  float a1 = sin(dlon) * cos(lat2);
  float a2 = sin(lat1) * cos(lat2) * cos(dlon);
  a2 = cos(lat1) * sin(lat2) - a2;
  a2 = atan2(a1, a2);
  if (a2 < 0.0) {
  	a2 += TWO_PI;			// modulo operator doesn't seem to work on floats
  }
  return degrees(a2);
}

int NMEA::_dehex(char a) {
	// returns base-16 value of chars '0'-'9' and 'A'-'F';
	// does not trap invalid chars!
  if (int(a) >= 65) {
    return int(a)-55;
  }
  else {
    return int(a)-48;
  }
}

float NMEA::_decimal(char* s) {
	// returns base-10 value of zero-termindated string
	// that contains only chars '+','-','0'-'9','.';
	// does not trap invalid strings!
  long  rl = 0;
  float rr = 0.0;
  float rb = 0.1;
  boolean dec = false;
  int i = 0;

  if ((s[i] == '-') || (s[i] == '+')) { i++; }
  while (s[i] != 0) {
    if (s[i] == '.') {
      dec = true;
    }
    else{
      if (!dec) {
        rl = (10 * rl) + (s[i] - 48);
      }
      else {
        rr += rb * (float)(s[i] - 48);
        rb /= 10.0;
      }
    }
    i++;
  }
  rr += (float)rl;
  if (s[0] == '-') {
    rr = 0.0 - rr;
  }
  return rr;
}
