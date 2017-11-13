#include <RunningAverage.h>

RunningAverage avg1; // initialize an instance of the class

void setup() {
  // no setup
}

void loop() {
  avg1.update(100, 1);
  delay(1000);
}
