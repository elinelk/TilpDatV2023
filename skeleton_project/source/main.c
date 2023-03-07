#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"
#include "modules/fsm.h"
#include "modules/timer.h"



int main(){
    elevio_init();
    elevator_initialize();

    while(1){

        {static int previous = -1;
        int current_floor = elevio_floorSensor();
        if(current_floor != -1 && current_floor != previous){
            fsm_FloorArrival(current_floor);
        }
        previous = current_floor; 
        }

        {static int previous_btn[N_FLOORS][N_BUTTONS];
        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                if (btnPressed && btnPressed != previous_btn[f][b]){
                    fsm_ButtonPress(f, b);
                }
                previous_btn[f][b] = btnPressed;
            }
        }}

        {if(timer_checkTimeOut()){
            timer_stop();
            fsm_Timeout();
        }
        }

        if(elevio_obstruction()){
            fsm_Obstruction();
        } 
        
        fsm_stop();
        
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
