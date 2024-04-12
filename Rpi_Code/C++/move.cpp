#include"gpio.cpp"
#include <iostream>


using namespace std;

typedef struct cSteps{
    float x_steps;
    float y_steps;
    float z_steps;
};

typedef struct mMovement{
    float x_steps;
    int Dirx;
    float y_steps;
    int Diry;
};

typedef struct position{
    float camera_x;
    float camera_y;
    float camera_z;
};

typedef struct rPosition{
    float robot_x;
    float robot_y;
    float robot_z;
};

gpio x_pulse(pulse_x,output);
gpio y_pulse(pulse_y,output);
gpio z_pulse(pulse_z,output);
gpio cutter(pulse_c,output);


gpio x_dir(dir_x,output);
gpio y_dir(dir_y,output);
gpio z_dir(dir_z,output);
gpio cutter_dir(dir_c,output);

gpio x_limit_front_switch(x_limit,input);
gpio x_limit_back_switch(x_limit_extra,input);
gpio y_limit_left_switch(y_limit,input);
gpio y_limit_right_switch(y_limit_extra,input);
gpio z_limit_switch(z_limit,input);


void origin(){
    x_dir.digitalWrite(x_back);
    y_dir.digitalWrite(y_left);
    z_dir.digitalWrite(z_up);

    for (int j=0;j<10*number_of_pulse_xy;j++){
        if(y_limit_left_switch.digitalRead()){
            y_pulse.digitalWrite(1);
            usleep(delay_y);
            y_pulse.digitalWrite(0);
            usleep(delay_y);
        }

          if(x_limit_back_switch.digitalRead()){
            x_pulse.digitalWrite(1);
            usleep(delay_x);
            x_pulse.digitalWrite(0);
            usleep(delay_x);
        }

          if(z_limit_switch.digitalRead()){
            z_pulse.digitalWrite(1);
            usleep(delay_z);
            z_pulse.digitalWrite(0);
            usleep(delay_z);
        }

        if(y_limit_left_switch.digitalRead()==0 && x_limit_back_switch.digitalRead()==0 &&z_limit_switch.digitalRead()==0){
            break;
        }
    }


}


void openclamp(){
    cutter_dir.digitalWrite(clamp_open);
    for(int i=0;i<0.2*number_of_pulse_zc;i++){
        cutter.digitalWrite(1);
        usleep(delay_c);
        cutter.digitalWrite(0);
        usleep(delay_c);
    }
}

void closeclamp(){
    cutter_dir.digitalWrite(clamp_close);
    for(int i=0;i<0.22*number_of_pulse_zc;i++){
        cutter.digitalWrite(1);
        usleep(delay_z);
        cutter.digitalWrite(0);
        usleep(delay_z);
    }
}

// void movexy(mMovement *movement){
//     x_dir.digitalWrite(movement->Dirx);
//     y_dir.digitalWrite(movement->Diry);
//     cout<<"x direction " << movement->Dirx <<"\n";
//      cout<<"y direction " << movement->Diry <<"\n";
//     int limit_value;
//     int pul_x=movement->x_steps*number_of_pulse_xy;
//     int pul_y=movement->y_steps*number_of_pulse_xy;

//     int maxP;
//     if(pul_x>pul_y){
//         maxP=pul_x;
//     }
//     else{
//         maxP=pul_y;
//     }
//     for(int i=0;i<=maxP;i++){
// 		if(i<=pul_y){
//             if(movement->Diry==y_left){
//                 limit_value=y_limit_left_switch.digitalRead();
//             }
//             else{
//                 limit_value=y_limit_right_switch.digitalRead();
//             }
//             if(limit_value==1){
//                 y_pulse.digitalWrite(1);
//                 usleep(delay_y);
//                 y_pulse.digitalWrite(0);
//                 usleep(delay_y);
//             }
		
// 		}

// 		if(i<=pul_x){
// 			if(movement->Dirx==x_front){
//                 limit_value=x_limit_front_switch.digitalRead();
//             }
//             else{
//                 limit_value=x_limit_back_switch.digitalRead();
//             }
//             if(limit_value==1){
//                 x_pulse.digitalWrite(1);
//                 usleep(delay_x);
//                 x_pulse.digitalWrite(0);
//                 usleep(delay_x);
//             }
// 		}
//     }
// }

void calculate_xy(position *camera_coordinate,cSteps *steps, rPosition *robot_pos){
    float cam_x=41+(camera_coordinate->camera_x);
	float cam_y=36+(camera_coordinate->camera_y);

    float frame_robot_x=20;
    float frame_robot_y=17;

    float error_x=0;
    float error_y=0;;

    // if(camera_coordinate->camera_x < 0 and camera_coordinate->camera_y<0){
    //     error_x=-3;
    //     error_y=-3;
    // }

    //  if(camera_coordinate->camera_x < 0 and camera_coordinate->camera_y > 0){
    //     error_x=-8;
    //     error_y=2;
    // }

    //  if(camera_coordinate->camera_x > 0 and camera_coordinate->camera_y>0){
    //     error_x=-4;
    //     error_y=1;
    // }

    cout<<"Error x = " <<error_x<<"\n";
    cout<<"Error y = " <<error_y<<"\n";


	// float robot_x=cam_x-10-20+8-1-10+1+19-20-4.5+8+error_x;
    float robot_x=cam_x-frame_robot_x+error_x;
	// float robot_y=cam_y-6-17-8+1+6.5+2-1+1+7+2-1-5+4+error_y;
    float robot_y=cam_y-frame_robot_y+error_y;
    cout<<"Robot X= " <<robot_x <<"\n";
    cout<<"Robot Y= " <<robot_y <<"\n";
    robot_pos->robot_x=robot_x;
    robot_pos->robot_y=robot_y;

    float step_x=robot_x/12.64;
	float step_y=robot_y/12.22;

    steps->x_steps=step_x;
    steps->y_steps=step_y;
}

void calculate_xy_movement(rPosition *req_pos,rPosition *prev_pos, mMovement *movement){
    if(req_pos->robot_x>prev_pos->robot_x){
        movement->Dirx=x_front;
        float diff=(req_pos->robot_x)-(prev_pos->robot_x);
        movement->x_steps=diff/12.64;
    }
    else{
        movement->Dirx=x_back;
        float diff=(prev_pos->robot_x)-(req_pos->robot_x);
        movement->x_steps=diff/12.64;
    }

    if(req_pos->robot_y>prev_pos->robot_y){
        movement->Diry=y_right;
        float diff=(req_pos->robot_y)-(prev_pos->robot_y);
        movement->y_steps=diff/12.22;
    }
    else{
        movement->Diry=y_left;
        float diff=(prev_pos->robot_y)-(req_pos->robot_y);
        movement->y_steps=diff/12.22;
    }

    cout<<" X motor dir = " <<movement->Dirx << "  and steps = "<<movement->x_steps <<"\n"; 
    cout<<" Y motor dir = " <<movement->Diry << "  and steps = "<<movement->y_steps <<"\n"; 
    cout<<"-------------\n";
}

void update_xy(rPosition *current_pos,rPosition *prev_pos){
    prev_pos->robot_x=0;
    prev_pos->robot_y=0;

    cout<<"current x= " <<prev_pos->robot_x <<" current y = "<<prev_pos->robot_y<<"\n";
    
}

void store_leave(){
    x_dir.digitalWrite(x_front);
    for (int i=0;i<5*number_of_pulse_xy;i++){
        if(x_limit_front_switch.digitalRead()){
            x_pulse.digitalWrite(1);
            usleep(delay_x);
            x_pulse.digitalWrite(0);
            usleep(delay_x);
        }
        else{
            break;
        }
    }

}


void move_z(int dir,float dis){
    z_dir.digitalWrite(dir);
    if(dir==z_up){
        for(int i=0;i<2*number_of_pulse_zc;i++){
            if(z_limit_switch.digitalRead()){
                z_pulse.digitalWrite(1);
                usleep(delay_z);
                z_pulse.digitalWrite(0);
                usleep(delay_z);
            }
            else{
                cout<<"Z pulse= "<<i<<"\n";
                break;
            }
        }
    }

    else{
            float step=dis/12.5;
            int total_pulse=step*number_of_pulse_zc;
            for(int i=0;i<1100;i++){
            
                z_pulse.digitalWrite(1);
                usleep(delay_z);
                z_pulse.digitalWrite(0);
                usleep(delay_z);
            
        }
    }
}

void move_x(mMovement *movement){
     x_dir.digitalWrite(movement->Dirx);
     int limit_value;
    int pul_x=movement->x_steps;
    for(int i=0;i<=(pul_x-100);i++){
            if(movement->Dirx==x_front){
                limit_value=x_limit_front_switch.digitalRead();
            }
            else{
                limit_value=x_limit_back_switch.digitalRead();
            }
            if(limit_value==1){
                x_pulse.digitalWrite(1);
                usleep(delay_x);
                x_pulse.digitalWrite(0);
                usleep(delay_x);
            }
    }

    for(int i=0; i<100;i++){
         x_pulse.digitalWrite(1);
         usleep(delay_x);
        x_pulse.digitalWrite(0);
        usleep(delay_x);
    }

}

void move_y(mMovement *movement){
     y_dir.digitalWrite(movement->Diry);
     int limit_value;
    // int pul_y=movement->y_steps*number_of_pulse_xy
    int pul_y=movement->y_steps;
    for(int i=0;i<=(pul_y-100);i++){
             if(movement->Diry==y_left){
                limit_value=y_limit_left_switch.digitalRead();
            }
            else{
                limit_value=y_limit_right_switch.digitalRead();
            }
            if(limit_value==1){
                y_pulse.digitalWrite(1);
                usleep(delay_y);
                y_pulse.digitalWrite(0);
                usleep(delay_y);
            }
    }

    for(int i=0;i<100;i++){
         y_pulse.digitalWrite(1);
        usleep(10000);
         y_pulse.digitalWrite(0);
         usleep(10000);
    }

}


void movexy(mMovement *movement){
    move_x(movement);
    move_y(movement);

}

void move_y_left(){
     
int diry=y_left; 
   y_dir.digitalWrite(diry);
     int limit_value;
    int pul_y=10*number_of_pulse_xy;
    for(int i=0;i<=pul_y;i++){
             if(diry==y_left){
                limit_value=y_limit_left_switch.digitalRead();
            }
            else{
                limit_value=y_limit_right_switch.digitalRead();
            }
            if(limit_value==1){
                y_pulse.digitalWrite(1);
                usleep(delay_y);
                y_pulse.digitalWrite(0);
                usleep(delay_y);
            }
               else{
              cout<<"Y pulse = "<<i<<"\n";
              break;
            }
    }

}


void move_y_right(){
    
int diry=y_right; 
   y_dir.digitalWrite(diry);
     int limit_value;
    int pul_y=10*number_of_pulse_xy;
    for(int i=0;i<=pul_y;i++){
             if(diry==y_left){
                limit_value=y_limit_left_switch.digitalRead();
            }
            else{
                limit_value=y_limit_right_switch.digitalRead();
            }
            if(limit_value==1){
                y_pulse.digitalWrite(1);
                usleep(delay_y);
                y_pulse.digitalWrite(0);
                usleep(delay_y);
            }
               else{
              cout<<i<<"\n";
              break;
            }
    }
}

void  move_x_back(){
    int dir=x_back;
      x_dir.digitalWrite(dir);
     int limit_value;
    int pul_x=10*number_of_pulse_xy;
    for(int i=0;i<=pul_x;i++){
            if(dir==x_front){
                limit_value=x_limit_front_switch.digitalRead();
            }
            else{
                limit_value=x_limit_back_switch.digitalRead();
            }
            if(limit_value==1){
                x_pulse.digitalWrite(1);
                usleep(delay_x);
                x_pulse.digitalWrite(0);
                usleep(delay_x);
           }
             else{
              cout<<"X pulse is ="<<i<<"\n";
              break;
            }

    }
}


int move_x_front(){
    int dir=x_front;
      x_dir.digitalWrite(dir);
     int limit_value;
    int pul_x=10*number_of_pulse_xy;
    for(int i=0;i<=pul_x;i++){
            if(dir==x_front){
                limit_value=x_limit_front_switch.digitalRead();
            }
            else{
                limit_value=x_limit_back_switch.digitalRead();
            }
            if(limit_value==1){
                x_pulse.digitalWrite(1);
                usleep(delay_x);
                x_pulse.digitalWrite(0);
                usleep(delay_x);
            }

            else{
                return i;
                break;
            }
    }
}

