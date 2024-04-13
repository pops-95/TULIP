#include "move.cpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

///camera x=207, y=273,for cutter minus 40 
// float pulse_per_dis_y=800/35.33;
float pulse_per_dis_x=800/60;
int main(){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> distribution(3, 5);

//     int camera_val_x=94;
//     int init_x=273+206-40;
//     int init_y=207;
     int init_y=447;
     int init_x=-275;
     int zero_x=203;
//     int camera_val_y=21;
    int error_x_posve=0;
     int error_x_negve=0;
    int error_y_posve=0;
    int error_y_negve=0;
   int error_y=0;//10;
   int error_x=-20;//-38;
   int initial_error=0;//-38;

    int camera_values[]={42,136,-34,23,-122,56,-165,212};
    // int camera_values[]={4,-113};
    // move_x_front();
    move_x_back();
    move_z(z_up,30);
    usleep(100000);

      move_y_left();
//     int temp_value=move_x_front();
    usleep(200000);
    // int prev_x_pulse=move_x_front();
    int full_x=830;
    int prev_x_dis=0;
    int prev_y_dis=0;
    int prev_x_pulse=0;
    int prev_y_pulse=0;
    long move_delay=3000000;

    mMovement val;

    /////////////////////////////////////

    //  int dis_x=init_x-camera_values[1];
    //  if(dis_x<0){
    //       dis_x=dis_x*(-1);
    //  }

    //  dis_x=dis_x+zero_x+error_x;
    //  // if(camera_values[0]<0){
    //  //      error_y=error_y_negve;
    //  // }
    //  // else{
    //  //      error_y=error_y_posve;
    //  // }
    //  int dis_y=init_y+error_y-camera_values[0];
    //  // val.y_steps=(dis_y)*pulse_per_dis_y;
    //  val.y_steps=(int)(0.0018*dis_y*dis_y)+(21.315*(dis_y))-4544.1;
    //  cout<<"distance y ="<<dis_y<<"\n";
    //  cout<<"Pulse required in y is = "<<val.y_steps<<"\n";
    //  val.x_steps=(13.603*(dis_x))-2620.4;
    //  cout<<"distance x ="<<dis_x<<"\n";
    //  cout<<"Pulse required in x is = "<<val.x_steps<<"\n";
    //  val.Dirx=x_front;
    //  val.Diry=y_right;
    //   move_x(&val);
    // move_y(&val);
    // move_z(z_down,30); 


    /////////////////////////////////////

    for(int i=0;i<4;i++){
         // Generate random number
        // double num = distribution(gen) * 10; // scale to range [-5, 5]
        move_z(z_up,40);
        
   
    //       if(camera_values[i*2]<0){
    //       error_y=error_y_negve;
    //  }
    //  else{
    //       error_y=error_y_posve;
    //  }


       int dis_x=init_x-camera_values[(i*2)+1];
     if(dis_x<0){
          dis_x=dis_x*(-1);
     }

     dis_x=dis_x+zero_x+error_x;
     // if(camera_values[0]<0){
     //      error_y=error_y_negve;
     // }
     // else{
     //      error_y=error_y_posve;
     // }
     int dis_y=init_y+error_y-camera_values[i*2];
        
        if(dis_x>prev_x_dis){
             val.Dirx=x_front;
        }
        else{
             val.Dirx=x_back;
        }

         if(dis_y>prev_y_dis){
             val.Diry=y_right;
        }
        else{
             val.Diry=y_left;
        }

        // int cal_pulse_x=(13.603*dis_x)-2620.4;
        // int cal_pulse_x=(13.741*(dis_x))-2679;
//          val.x_steps=(prev_x_dis-dis_x)*pulse_per_dis_x;
//          if(val.x_steps<0){
//             val.x_steps=val.x_steps*(-1);
//          }

        
//     val.y_steps=(dis_y-prev_y_dis)*pulse_per_dis_y;
//      if(val.y_steps<0){
//             val.y_steps=val.y_steps*(-1);
//          }
    //  int req_y_pulse=(int)(0.0018*dis_y*dis_y)+(21.315*(dis_y))-4544.1;
    int req_y_pulse=(int)((dis_y-214.6)/0.0429);
    // int req_y_pulse=(dis_y-prev_y_dis)*pulse_per_dis_y;
    //  val.y_steps=(req_y_pulse);
     val.y_steps=(req_y_pulse)-prev_y_pulse;
           if(val.y_steps<0){
            val.y_steps=val.y_steps*(-1);
         }
              cout<<"distance y ="<<dis_y<<"\n";
          cout<<"Pulse required in y is = "<<val.y_steps<<"\n";

    if(prev_x_dis!=full_x){
          int req_x_pulse=(13.603*(dis_x))-2620.4;
          val.x_steps=(req_x_pulse)-prev_x_pulse;
    }

    else{
         int req_x_pulse=(-13.302*(dis_x))+11038;
          val.x_steps=(req_x_pulse)-prev_x_pulse;
    }
    
       
         if(val.x_steps<0){
            val.x_steps=val.x_steps*(-1);
         }
          cout<<"distance x ="<<dis_x<<"\n";
          cout<<"Pulse required in x is = "<<val.x_steps<<"\n";

    // move_x(&val);
    // move_y(&val);
    movexy(&val);
    // check_y(&val,prev_y_dis,dis_y);

    move_z(z_down,30); 
    
    usleep(move_delay);
     move_z(z_up,30);

    move_x_front();
    usleep(3000000);

    prev_x_pulse=0;
    usleep(10000);
    prev_x_dis=full_x;
    prev_y_pulse=req_y_pulse;
    prev_y_dis=dis_y;

    // error_x=error_x-20;
    if((i+1)%2==0){
        error_y=error_y+15;
        error_x=error_x-10;
    }
    
    }



    ///////////////////////////////////////////////////////////////////


    

        

}