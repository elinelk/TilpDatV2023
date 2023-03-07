#include "requests.h"

int isAbove(ELEVATOR e){
    for (int f = e.floor+1; f <N_FLOORS; f++){
        for(int btn = 0; btn < N_BUTTONS; btn++){
            if(e.request[f][btn]){
                return 1;
            }
        }
    }
    return 0;
}

int isBelow(ELEVATOR e){
    for (int f = 0; f <e.floor; f++){
        for(int btn = 0; btn < N_BUTTONS; btn++){
            if(e.request[f][btn]){
                return 1;
            }
        }
    }
    return 0;
}
 
int isHere(ELEVATOR e){
    for(int btn = 0; btn < N_BUTTONS; btn++){
        if(e.request[e.floor][btn]){
            return 1;
        }
    }
    return 0;
}

int Should_Stop(ELEVATOR e){
    switch(e.direction){
        case DIRN_DOWN:
        return e.request[e.floor][BUTTON_HALL_DOWN] || e.request[e.floor][BUTTON_CAB] || !isBelow(e); //Should only stop if the passengers is going in the same direction
        case DIRN_UP:
        return e.request[e.floor][BUTTON_HALL_UP] || e.request[e.floor][BUTTON_CAB] || !isAbove(e);
        default:
        return 1;
    }
}



Elev_Behaviour chooseDirection(ELEVATOR e){
    switch(e.direction){
        case DIRN_DOWN:
            if (isBelow(e)){
                return (Elev_Behaviour){DIRN_DOWN, MOVING};
            }
            else if (isHere(e)){
                return (Elev_Behaviour){DIRN_UP, DOOR_OPEN};
            }
            else if (isAbove(e)){
                return (Elev_Behaviour){DIRN_UP, MOVING};
            }
            
            else{
                return (Elev_Behaviour){DIRN_STOP, IDLE};
            }
        case DIRN_UP:
            if (isAbove(e)){
                return (Elev_Behaviour){DIRN_UP, MOVING};
            }
            else if (isHere(e)){
                return (Elev_Behaviour){DIRN_DOWN, DOOR_OPEN};
            }
            else if (isBelow(e)){
                return (Elev_Behaviour){DIRN_DOWN, MOVING};
            }
            else{
                return (Elev_Behaviour){DIRN_STOP, IDLE};
            }
        case DIRN_STOP:
            if (isHere(e)){
                return (Elev_Behaviour){DIRN_STOP, DOOR_OPEN};
            }
            else if (isAbove(e)){
                return (Elev_Behaviour){DIRN_UP, MOVING};
            }
            else if (isBelow(e)){
                return (Elev_Behaviour){DIRN_DOWN, MOVING};
            }
            else{
                return (Elev_Behaviour){DIRN_STOP, IDLE};
            }
        default: 
            return (Elev_Behaviour){DIRN_STOP, IDLE};
    }

}

ELEVATOR clear_Requests(ELEVATOR e){
    for(int btn = 0; btn < N_BUTTONS; btn++){
        e.request[e.floor][btn]=0;
    }
    return e;
}