#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>

#include "timer.h"

double timer::getElapsedSeconds(timespec *start, timespec *end){
  long elapsedSeconds = end->tv_sec - start->tv_sec;
  long elapsedNanoseconds = end->tv_nsec - start->tv_nsec;
  if(elapsedNanoseconds < 0){
    elapsedSeconds --;
    elapsedNanoseconds = BILLION + elapsedNanoseconds;
  }
  //convert to total time elapsed in seconds
  float timeElapsed = elapsedSeconds + (float(elapsedNanoseconds)/BILLION);
  return timeElapsed;
}

double timer::getElapsedSeconds(timespec *start){
  timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  return getElapsedSeconds(start, &now);
}

double timer::getElapsedNanoSeconds(timespec *start, timespec *end){
  double elapsedSeconds = getElapsedSeconds(start, end);
  return elapsedSeconds *  BILLION;
}
double timer::getElapsedNanoSeconds(timespec *start){
  timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  return getElapsedNanoSeconds(start, &now);
}
