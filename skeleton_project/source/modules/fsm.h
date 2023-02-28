#pragma once

#include "elevio.h"

typedef enum {
    IDLE,
    DOOR_OPEN,
    MOVING,
    STOP
} ELEV_STATE;

typedef struct{
    int floor;
    MotorDirection dirn;
    int request[N_FLOORS][N_BUTTONS];
    ELEV_STATE state;
    double doorOpenDuration;
} ELEVATOR;

ELEVATOR elevator_initialize(){
    return (ELEVATOR){
        .floor = -1,
        .dirn = DIRN_STOP,
        .state = IDLE,
        .doorOpenDuration = 3.0 
    }
};