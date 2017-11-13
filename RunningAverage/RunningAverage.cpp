#include "RunningAverage.h" // include the declaration for this class

// <<constructor>>
RunningAverage::RunningAverage() { /* nothing to construct */ }

// <<destructor>>
RunningAverage::~RunningAverage() { /* nothing to destruct */ }

// update the average and return the current value
float RunningAverage::update(float input, float slope)
{ 
  // subtract the last reading:
  _total = _total - _history[_index];
  // read from the sensor:  
  _history[_index] = input;
  // add the reading to the total:
  _total= _total + _history[_index];
  // advance to the next position in the array:  
  _index = _index + 1;

  // if we're at the end of the array...
  if (_index >= slope) {
    // ...wrap around to the beginning: 
    _index = 0;
  }

  // calculate the average:
  _average = _total / slope;
  return _average;
}
