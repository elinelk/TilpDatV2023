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
    MotorDirection direction;
    int request[N_FLOORS][N_BUTTONS];
    ELEV_STATE state;
    double doorOpenDuration;
} ELEVATOR;

ELEVATOR elevator_initialize(void);
void setLights(ELEVATOR e);
void fsm_InitBetweenFloors(void);
void fsm_ButtonPress(int btnFloor, ButtonType btnType);
void fsm_FloorArrival(int floor);
void fsm_Timeout(void);