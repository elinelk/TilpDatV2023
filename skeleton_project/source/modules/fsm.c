#include "fsm.h"
#include "requests.h"
#include "../driver/elevio.h"
#include "timer.h"

static ELEVATOR elevator;

ELEVATOR elevator_initialize(){
    ELEVATOR elevator = (ELEVATOR){
        .floor = elevio_floorSensor(),
        .direction = DIRN_STOP,
        .state = IDLE,
        .doorOpenDuration = 3.0 
    };
    setLights(elevator);
    elevio_doorOpenLamp(0);
    if (elevator.floor==-1){
        fsm_InitBetweenFloors();
    }
    return elevator;
};


void fsm_InitBetweenFloors(void){
    elevio_motorDirection(DIRN_DOWN);
    elevator.state = MOVING;
    elevator.direction = DIRN_DOWN;
}

void elevator_print(ELEVATOR es){
    printf("  +--------------------+\n");
    printf("  |  | up  | dn  | cab |\n");
    for(int f = N_FLOORS-1; f >= 0; f--){
        printf("  | %d", f);
        for(int btn = 0; btn < N_BUTTONS; btn++){
            if((f == N_FLOORS-1 && btn == BUTTON_HALL_UP)  || 
               (f == 0 && btn == BUTTON_HALL_DOWN) 
            ){
                printf("|     ");
            } else {
                printf(es.request[f][btn] ? "|  #  " : "|  -  ");
            }
        }
        printf("|\n");
    }
    printf("  +--------------------+\n");
}

void setLights(ELEVATOR e){
    for (int f = 0; f <N_FLOORS; f++){
        for(int btn = 0; btn < N_BUTTONS; btn++){
            elevio_buttonLamp(f,btn,e.request[f][btn]);
        }
    }
}

void fsm_ButtonPress(int btnFloor, ButtonType btnType){
    switch (elevator.state){
    case DOOR_OPEN:
        if(btnFloor==elevator.floor){
            timer_start(elevator.doorOpenDuration);
        }
        else{
            elevator.request[btnFloor][btnType]=1;
        }
        break;
    case MOVING:
        elevator.request[btnFloor][btnType]=1;
        break;
    
    case IDLE:
        elevator.request[btnFloor][btnType]=1;
        Elev_Behaviour newBehaviour = chooseDirection(elevator);
        elevator.direction = newBehaviour.direction;
        elevator.state = newBehaviour.state;
        switch (elevator.state)
        {
        case DOOR_OPEN:
            elevio_doorOpenLamp(1);
            timer_start(elevator.doorOpenDuration);
            elevator = clear_Requests(elevator);
            break;
        case MOVING:
            elevio_motorDirection(elevator.direction);
            break;
        case IDLE:
            break;
        default:
            break;
        }
        break;
    case STOP:
        break;
    }
    setLights(elevator);
}

void fsm_FloorArrival(int floor){
    elevator.floor = floor;
    elevio_floorIndicator(floor);

    switch (elevator.state)
    {
    case MOVING:
        if(Should_Stop(elevator)){
            elevio_motorDirection(DIRN_STOP);
            elevio_doorOpenLamp(1);
            timer_start(elevator.doorOpenDuration);
            elevator = clear_Requests(elevator);
            setLights(elevator);
            elevator.state = DOOR_OPEN;
        }
        break;
    
    default:
        break;
    }

}

void fsm_Timeout(void){
    switch (elevator.state)
    {
    case DOOR_OPEN:;
        Elev_Behaviour newBehaviour = chooseDirection(elevator);
        elevator.direction = newBehaviour.direction;
        elevator.state = newBehaviour.state;

        switch(elevator.state){
            case DOOR_OPEN:
                timer_start(elevator.doorOpenDuration);
                elevator = clear_Requests(elevator);
                setLights(elevator);
                break;
            case MOVING:
            case IDLE:
                elevio_doorOpenLamp(0);
                elevio_motorDirection(elevator.direction);
                break;
        }
        break;
    
    default:
        break;
    }
}

void fsm_Obstruction(void){
    switch (elevator.state)
    {
    case DOOR_OPEN:
        timer_start(elevator.doorOpenDuration);
        break;
    
    default:
        break;
    }
}

void fsm_stop(void){
    switch (elevator.state)
    {
    case STOP:
        if(!elevio_stopButton()){
            elevator.state = IDLE;
            elevio_stopLamp(0);
            if (elevio_floorSensor()!=-1){
                elevator.state = DOOR_OPEN;
                timer_start(elevator.doorOpenDuration);
            }
        }
        break;
    
    default:
        if(elevio_stopButton()){
            elevator.state = STOP;
            elevio_motorDirection(DIRN_STOP);
            elevio_stopLamp(1);

            for (int f = 0; f < N_FLOORS; f++){
                for(int btn = 0; btn < N_BUTTONS; btn++){
                    elevator.request[f][btn]=0;
                }
            }
            setLights(elevator);
            if (elevio_floorSensor()!=-1){
                elevio_doorOpenLamp(1);
            }
        }
        break;
    }
    
}