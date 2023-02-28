#include <stdio.h>
#include <sys/time.h>
#include "timer.h"

static  double  timerStopTime;
static  int     timerActive;

static double get_current_time(void) {
    return (double)time(NULL);
}

void timer_start(double duration){
    timerStopTime = get_current_time() + duration;
    timerActive = 1;

    printf(timerStopTime, timerActive)
}

void timer_stop(void){
    timerActive = 0;
}

void timer_checkTimeOut(void){
    if (get_current_time() > timerStopTime && timerActive){
        return 1;
    }
    else {
        return 0;
    }
}