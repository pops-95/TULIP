#include "move.cpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

///camera x=207, y=273,for cutter minus 40 
// float pulse_per_dis_y=800/35.33;

int main(){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> distribution(3, 5);

//     int camera_val_x=94;
//     int init_x=273+206-40;
//     int init_y=207;
     int init_y=461;
     int init_x=-263;
     int zero_x=212;
//     int camera_val_y=21;
    int error_x_posve=0;
     int error_x_negve=0;
    int error_y_posve=0;
    int error_y_negve=0;
   int error_y=0;//10;
   int error_x=-20;//-38;
   int new_error_x=-20;
   int new_error_y=-20;
   int initial_error=0;//-38;

    int camera_values[]={190,76,1172};
    int no_values=(sizeof(camera_values)/sizeof(camera_values[0]))/3;
    cout<<"No of values="<<no_values<<endl;
    // int camera_values[]={4,-113};
    // move_x_front();
    // openclamp();
//     closeclamp();
    move_x_back();
      move_z(z_up,30);
    usleep(100000);

      move_y_left();
//     int temp_value=move_x_front();
    usleep(200000);
    // int prev_x_pulse=move_x_front();
    
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

    //  dis_x=dis_x+zero_x+error_x+30;
    //  // if(camera_values[0]<0){
    //  //      error_y=error_y_negve;
    //  // }
    //  // else{
    //  //      error_y=error_y_posve;
    //  // }
    //  int dis_y=init_y+error_y-camera_values[0]-20;
    //  // val.y_steps=(dis_y)*pulse_per_dis_y;
    //  val.y_steps=(22.474*(dis_y))-4222.5;
    //  cout<<"distance y ="<<dis_y<<"\n";
    //  cout<<"Pulse required in y is = "<<val.y_steps<<"\n";
    //  val.x_steps=(dis_x-zero_x)*pulse_per_dis_x;
    //  cout<<"distance x ="<<dis_x<<"\n";
    //  cout<<"Pulse required in x is = "<<val.x_steps<<"\n";
    //  val.Dirx=x_front;
    //  val.Diry=y_right;
    //   move_x(&val);
    // move_y(&val);
    // int change=camera_values[2]-1060+100;
    // openclamp();
    
    // usleep(10000);
    // move_z(z_down,change); 
    // usleep(1000000);
    // closeclamp();

    // move_z(z_up,30);

    // store_leaves_belt(dis_x);
    // usleep(10000);

    // openclamp();

    // usleep(2000000);

    // closeclamp();


    /////////////////////////////////////

    for(int i=0;i<no_values;i++){
         // Generate random number
        // double num = distribution(gen) * 10; // scale to range [-5, 5]
        move_z(z_up,40);
        
   
    //       if(camera_values[i*2]<0){
    //       error_y=error_y_negve;
    //  }
    //  else{
    //       error_y=error_y_posve;
    //  }


       int dis_x=init_x-camera_values[(i*3)+1];
     if(dis_x<0){
          dis_x=dis_x*(-1);
     }

     dis_x=dis_x+zero_x+error_x+new_error_x;
     // if(camera_values[0]<0){
     //      error_y=error_y_negve;
     // }
     // else{
     //      error_y=error_y_posve;
     // }
     int dis_y=init_y+error_y-camera_values[i*3]+new_error_y;
        
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
    int req_y_pulse=(22.474*(dis_y))-4222.5;
    // int req_y_pulse=(dis_y-prev_y_dis)*pulse_per_dis_y;
    //  val.y_steps=(req_y_pulse);
     val.y_steps=(req_y_pulse)-prev_y_pulse;
           if(val.y_steps<0){
            val.y_steps=val.y_steps*(-1);
         }
              cout<<"distance y ="<<dis_y<<"\n";
          cout<<"Pulse required in y is = "<<val.y_steps<<"\n";

    if(prev_x_dis==full_x){
          
          val.x_steps=(full_x-dis_x)*pulse_per_dis_x;
    }

    else{
         
          val.x_steps=(dis_x-zero_x)*pulse_per_dis_x;
    }
    
       
         if(val.x_steps<0){
            val.x_steps=val.x_steps*(-1);
         }
          cout<<"distance x ="<<dis_x<<"\n";
          cout<<"Pulse required in x is = "<<val.x_steps<<"\n";

    move_x(&val);
    move_y(&val);
    // movexy(&val);
    // check_y(&val,prev_y_dis,dis_y);
    int change=camera_values[(i*3)+2]-1060;

    openclamp();
    usleep(100000);
    move_z(z_down,change); 

    
    
    usleep(1000000);
     closeclamp();

    move_z(z_up,40);

    store_leaves_belt(dis_x);
    usleep(10000);

    openclamp();

    usleep(2000000);

    closeclamp();

    prev_x_pulse=0;
    usleep(10000);
    prev_x_dis=full_x;
    prev_y_pulse=req_y_pulse;
    prev_y_dis=dis_y;

    // error_x=error_x-20;
    // if((i+1)%2==0){
    //     error_y=error_y+15;
    //     error_x=error_x-10;
    // }
    
    }



    ///////////////////////////////////////////////////////////////////


    

        

}