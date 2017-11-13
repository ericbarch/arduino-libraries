#ifndef RUNNINGAVERAGE_H
#define RUNNINGAVERAGE_H

class RunningAverage { 
  public:
    RunningAverage();
    ~RunningAverage();
    float update(float input, float slope);
  private:
    float _history[1000];
    float _total = 0.0f;
    int _index = 0;
    float _average = 0.0f;
};

#endif