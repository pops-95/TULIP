#include "move.cpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

///camera x=207, y=273,for cutter minus 40 
float pulse_per_dis_y=800/35.33;
float pulse_per_dis_x=800/60;
int main(){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> distribution(3, 5);

    int camera_val_x=94;
//     int init_x=273+206-40;
//     int init_y=207;
     int init_x=439;
     int init_y=416;
     int zero_y=206;
    int camera_val_y=21;
    int error_x=18;
    int error_y=35;
   

    int camera_values[]={93,106};
    // int camera_values[]={4,-113};
    // move_x_front();
    move_x_back();
    move_z(z_up,30);

      move_y_left();
//     int temp_value=move_x_front();
    usleep(100000);
    // int prev_x_pulse=move_x_front();
    int full_x=830;
    int prev_x_dis=0;
    int prev_y_dis=0;
    int prev_x_pulse=0;
    int prev_y_pulse=0;
    long move_delay=10000000;

    mMovement val;

    /////////////////////////////////////

     int dis_x=init_x+error_x+camera_values[0];
     int dis_y=init_y-zero_y+error_y-camera_values[1];
     val.y_steps=(dis_y)*pulse_per_dis_y;
     val.x_steps=(13.603*(dis_x))-2620.4;
     val.Dirx=x_front;
     val.Diry=y_right;
      move_x(&val);
    move_y(&val);
    move_z(z_down,30); 


    /////////////////////////////////////

//     for(int i=0;i<=2;i++){
//          // Generate random number
//         // double num = distribution(gen) * 10; // scale to range [-5, 5]
//         move_z(z_up,40);
        
//          int dis_x=init_x+error_x-camera_values[(i*2)+1];
//          int dis_y=init_y+error_y-camera_values[i*2];
        
//         if(dis_x>prev_x_dis){
//              val.Dirx=x_back;
//         }
//         else{
//              val.Dirx=x_front;
//         }

//          if(dis_y>prev_y_dis){
//              val.Diry=y_right;
//         }
//         else{
//              val.Diry=y_left;
//         }

//         // int cal_pulse_x=(13.603*dis_x)-2620.4;
//         // int cal_pulse_x=(13.741*(dis_x))-2679;
//          val.x_steps=(prev_x_dis-dis_x)*pulse_per_dis_x;
//          if(val.x_steps<0){
//             val.x_steps=val.x_steps*(-1);
//          }

        
//     val.y_steps=(dis_y-prev_y_dis)*pulse_per_dis_y;
//      if(val.y_steps<0){
//             val.y_steps=val.y_steps*(-1);
//          }

//     move_x(&val);
//     move_y(&val);

//     move_z(z_down,30); 
    
//     usleep(move_delay);
//      move_z(z_up,30);

//     prev_x_pulse=move_x_front();
//     usleep(10000);
//     prev_x_dis=0;
//     // prev_x_pulse=val.x_steps;
//     prev_y_dis=dis_y;
    
//     }

        

}