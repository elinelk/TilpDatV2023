#include <stdio.h>
#include <sys/time.h>
#include "timer.h"

static double  timerStopTime;
static int     timerActive;

void timer_start(double duration){
    timerStopTime = time(NULL) + 3.0;
    timerActive = 1;
}

void timer_stop(void){
    timerActive = 0;
}

int timer_checkTimeOut(void){
    if (timerActive && (time(NULL) > timerStopTime )){
        return 1;
    }
    else {
        return 0;
    }
}