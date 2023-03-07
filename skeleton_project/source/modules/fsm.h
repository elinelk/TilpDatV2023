#pragma once

#include "../driver/elevio.h"

typedef enum {
    IDLE =0 ,
    DOOR_OPEN = 1,
    MOVING = 2,
    STOP = 3
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
void elevator_print(ELEVATOR es);
void fsm_Obstruction(void);
void fsm_stop(void);