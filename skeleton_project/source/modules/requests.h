#pragma once

#include "../driver/elevio.h"
#include "fsm.h"

typedef struct {
    MotorDirection direction;
    ELEV_STATE state;
} Elev_Behaviour;


int Should_Stop(ELEVATOR e);
int isAbove(ELEVATOR e);
int isBelow(ELEVATOR e);
int isHere(ELEVATOR e);
Elev_Behaviour chooseDirection(ELEVATOR e);
ELEVATOR clear_Requests(ELEVATOR e);
