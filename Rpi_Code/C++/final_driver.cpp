#include "move.cpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

#define pulse_per_dis_x (800/60)
#define  pulse_per_dis_y (800/35.33)
#define init_x 439
#define init_y 207

int main(){
    rPosition current_pos,prev_pos;
    position camera_coor;
    mMovement motor_move;

    int values[]={};
    int no_values=(sizeof(values)/sizeof(values[0]))/2;
    cout<<"values = "<<no_values<<"\n";

    origin();
    


}