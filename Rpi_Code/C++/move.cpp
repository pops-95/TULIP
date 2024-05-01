#include"gpio.cpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include<atomic>

// Mutex for synchronization
std::mutex distanceMutex;
std:: mutex y_distance;

float pulse_per_dis_x=800/60;


int full_x=863;


using namespace std;

typedef struct cSteps{
    float x_steps;
    float y_steps;
    float z_steps;
};

typedef struct mMovement{
    float x_distance;
    int Dirx;
    float x_steps;
    float y_distance;
    float y_steps;
    int Diry;
    float z_steps;
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

typedef struct prevPosition{
    int prev_x;
    int prev_y;
};

typedef struct Distances{
    float x_distance;
    float y_distance;
};

typedef struct Errordata{
    float accu_x;
    float accu_y;
};

typedef struct Initial_Measeures{
    int y_pulse_right;
    int y_pulse_left;
    int x_pulse_front;
    int x_pulse_back;
    float y_left_extreme_distance;
    float y_right_extreme_distance;
    float x_front_extreme_distance;
    float x_back_extreme_distance;

};

struct ThreadData {
    pthread_mutex_t mtx; // Mutex for synchronization
    int stopFlag; // Flag to stop the thread
    Distances dis;
};

std::atomic<int> sharedData_x; // Shared data variable
std::atomic<int> sharedData_y; // Shared data variable

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

// gpio x_echo(x_echo_pin,input);
// gpio x_trigger(x_trigger_pin,output);
// gpio y_echo(y_echo_pin,input);
// gpio y_trigger(y_trigger_pin,output);


void origin(){
    x_dir.digitalWrite(x_back);
    y_dir.digitalWrite(y_left);
    z_dir.digitalWrite(z_up);

    for (int j=0;j<10*number_of_pulse_xy;j++){
        if(y_limit_left_switch.digitalRead()==0){
            y_pulse.digitalWrite(1);
            usleep(delay_y);
            y_pulse.digitalWrite(0);
            usleep(delay_y);
        }

          if(x_limit_back_switch.digitalRead()==0){
            x_pulse.digitalWrite(1);
            usleep(delay_x);
            x_pulse.digitalWrite(0);
            usleep(delay_x);
        }

          if(z_limit_switch.digitalRead()==0){
            z_pulse.digitalWrite(1);
            usleep(delay_z);
            z_pulse.digitalWrite(0);
            usleep(delay_z);
        }

        if(y_limit_left_switch.digitalRead()==1 && x_limit_back_switch.digitalRead()==1 &&z_limit_switch.digitalRead()==1){
            break;
        }
    }


}


void openclamp(){
    cutter_dir.digitalWrite(clamp_open);
    for(int i=0;i<0.3*number_of_pulse_zc;i++){
        cutter.digitalWrite(1);
        usleep(delay_c);
        cutter.digitalWrite(0);
        usleep(delay_c);
    }
}

void closeclamp(){
    cutter_dir.digitalWrite(clamp_close);
    for(int i=0;i<0.6*number_of_pulse_zc;i++){
        cutter.digitalWrite(1);
        usleep(delay_z);
        cutter.digitalWrite(0);
        usleep(delay_z);
    }
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
            float step=dis/(125*2);
            int total_pulse=step*number_of_pulse_zc;
            for(int i=0;i<total_pulse;i++){
            
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
            if(limit_value==0){
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
            if(limit_value==0){
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
            cout<<"current y value= "<<y_limit_left_switch.digitalRead()<<endl;
            //  if(diry==y_left){
            //     limit_value=y_limit_left_switch.digitalRead();
            // }
            // else{
            //     limit_value=y_limit_right_switch.digitalRead();
            // }
            if(y_limit_left_switch.digitalRead()==0 &&  y_limit_right_switch.digitalRead()==0){
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
            if(limit_value==0){
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
            // cout<<limit_value;
            if(limit_value==0){
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


void move_x_front(){
    int dir=x_front;
    x_dir.digitalWrite(dir);
     int limit_value;
    int pul_x=100*number_of_pulse_xy;
    for(int i=0;i<=pul_x;i++){
            if(dir==x_front){
                limit_value=x_limit_front_switch.digitalRead();
            }
            else{
                limit_value=x_limit_back_switch.digitalRead();
            }
            if(limit_value==0){
                x_pulse.digitalWrite(1);
                usleep(delay_x);
                x_pulse.digitalWrite(0);
                usleep(delay_x);
            }

            else{
                // return i;
                break;
            }
    }
}

void store_leaves_belt(int current_pos){
    int dir=x_front;
    x_dir.digitalWrite(dir);
    int diff=863-current_pos;
    int steps=diff*pulse_per_dis_x;
    for (int i=0;i<steps;i++){
         x_pulse.digitalWrite(1);
                usleep(delay_x);
                x_pulse.digitalWrite(0);
                usleep(delay_x);
    }

}






void calculate_xy(position& cam_val,rPosition& bot_val,mMovement& move){
    int dis_x=ref_x-cam_val.camera_x;
     if(dis_x<0){
          dis_x=dis_x*(-1);
     }

     move.x_distance=full_sensor_x-( dis_x+zero_x+error_x+new_error_x+zero_y);

    move.y_distance=ref_y+error_y-cam_val.camera_y+new_error_y;
    
    if(move.x_distance>bot_val.robot_x){
            move.Dirx=x_front;
    }
    else{
            move.Dirx=x_back;
    }

        if(move.y_distance>bot_val.robot_y){
            move.Diry=y_right;
    }
    else{
            move.Diry=y_left;
    }

    move.z_steps=cam_val.camera_z-1060;
    
}


void move_x_sonar(mMovement& val,Distances& dis){
    cout<<"X SONAR CALLED"<<endl;
    int limit;
    if(dis.x_distance>val.x_distance){
        x_dir.digitalWrite(x_front);
    }
    else{
        x_dir.digitalWrite(x_back);
    }
    
    int minmumDistance=val.x_distance-reach_threshold;
    int maximumDistane=val.x_distance+reach_threshold;
    
    
    bool reached=false;
    cout<<"X distance outside loop= "<< dis.x_distance << "  "<<minmumDistance<<"  "<<maximumDistane<<endl;
    while(!reached){

        
        // cout<<"X distance= "<< dis.x_distance <<endl;
        
         if(x_limit_back_switch.digitalRead()==0 || x_limit_front_switch.digitalRead()==0){
                x_pulse.digitalWrite(1);
                usleep(delay_x);
                x_pulse.digitalWrite(0);
                usleep(delay_x);
            }
        usleep(100);
           
        // lock.unlock();
        unique_lock<mutex> lock(distanceMutex,defer_lock);
        lock.lock();
        if(minmumDistance<=dis.x_distance && maximumDistane >=dis.x_distance){
            cout<<"X distance inside-----= "<< dis.x_distance <<endl;
            reached=true;
            break;
        }
        lock.unlock();
        


        
        
        
           
            
            // usleep(100000);
        
        
    }
    // lock.unlock();

}


void move_y_sonar(mMovement& val,Distances& dis){
     if(dis.y_distance>val.y_distance){
        y_dir.digitalWrite(y_left);
    }
    else{
        y_dir.digitalWrite(y_right);
    }
    int minmumDistance=val.y_distance-reach_threshold;
    int maximumDistane=val.y_distance+reach_threshold;
    
    
    bool reached=false;
     cout<<"  Y Distance outside loop= "<<dis.y_distance<< "  "<<minmumDistance<<"  "<<maximumDistane<<endl;
    while(!reached){
        
       
        cout<<"  Y Distance = "<<dis.y_distance<<endl;
       
            if(y_limit_left_switch.digitalRead()==0&&y_limit_right_switch.digitalRead()==0){
            y_pulse.digitalWrite(1);
            usleep(delay_y);
            y_pulse.digitalWrite(0);
            usleep(delay_y);
            }
            unique_lock<mutex> lock(y_distance,defer_lock);
            lock.lock();
            if(minmumDistance<=dis.y_distance && maximumDistane >=dis.y_distance){
             cout<<"  Y Distance Inside ------ = "<<dis.y_distance<<endl;
            reached=true;
            break;
            
             }
            lock.unlock();
           
        }
        
    

}


void check_accuracy(mMovement& move_val,Distances& val,Errordata& data ){
	float acc_err_x=move_val.x_distance-val.x_distance;
    // if(acc_err_x<0){
    //     acc_err_x=acc_err_x*(-1);
    // }
    data.accu_x=acc_err_x;

    float acc_err_y=move_val.y_distance-val.y_distance;
    // if(acc_err_y<0){
    //     acc_err_y=acc_err_y*(-1);
    // }

    data.accu_y=acc_err_y;

}


void fine_tuning_xy(Errordata& data,mMovement& move_val){
    if(data.accu_x>0){
        move_val.Dirx=x_back;
    }

    else{
        move_val.Dirx=x_front;
    }

    if(data.accu_y>0){
        move_val.Diry=y_right;
    }

    else{
        move_val.Diry=y_left;
    }

    move_val.x_steps=data.accu_x;
    move_val.y_steps=data.accu_y;

}   








