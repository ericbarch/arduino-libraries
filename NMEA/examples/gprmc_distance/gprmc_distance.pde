// This example illustates basic use of the NMEA library.
// It assumes that a GPS receiver is connected to serial
// port 'Serial1' at 4800 bps, and that a LED is connected
// to digital i/o pin 0.
//
// A GPS data connection of type GPRMC is created, and
// used to get distance to a destination. If the distance
// is under 500 meters, the LED lights up, otherwise it
// is off.

#include <nmea.h>

NMEA gps(GPRMC);  // GPS data connection to GPRMC sentence type

// destination coordinates in degrees-decimal
float dest_latitude = 48.858342;
float dest_longitude = 2.294522;

void setup() {
  Serial1.begin(4800);
  pinMode(0, OUTPUT);
}

void loop() {
  if (Serial1.available() > 0 ) {
    // read incoming character from GPS
    char c = Serial1.read();

    // check if the character completes a valid GPS sentence
    if (gps.decode(c)) {
      // check if GPS positioning was active
      if (gps.gprmc_status() == 'A') {
        // read distance to destination in meters and set led accordingly
        if (gps.gprmc_distance_to(dest_latitude, dest_longitude, MTR) < 500.0) {
          digitalWrite(0, HIGH);
        }
        else {
          digitalWrite(0, LOW);
        }
      }
    }
  }
}
