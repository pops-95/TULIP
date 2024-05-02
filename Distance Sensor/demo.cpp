#include "ultrasonic.cpp"
#include <iostream>
#include <thread>
// #include "../Rpi_Code/C++/move.cpp"
#include <chrono>



using namespace std;

bool flag_x=true;
bool flag_y=false;


Distances dis;
mMovement move_val;
int desired_dis=330;


uint16_t Dev_x;
uint16_t Dev_y;

uint8_t add=0x44;
uint8_t change_add=add>>1;


int main(int argc, char const *argv[])
{   
    y_shut.digitalWrite(1);
    x_shut.digitalWrite(1);
    change_sensor_add(Dev_y,add);
    // y_shut.digitalWrite(1);

    // sensor_start(Dev_y,change_add);

    // sensor_start(Dev_x,I2cDevAddr);
    // sensor_start(Dev_x);
    thread x_distance(measurement,ref(dis),ref(Dev_x),ref(flag_x),ref(I2cDevAddr));
    thread y_distance(measurement,ref(dis),ref(Dev_y),ref(flag_y),ref(change_add));
    // // while (1)
    // {
    //     /* code */
    //     cout<<dis.x_distance<<endl;
    // }
    // while(1){
    //     x_shut.digitalWrite(1);
    //     y_shut.digitalWrite(1);
    // }

    

    // move_val.x_distance=atoi(argv[1]);
    // move_val.y_distance=atoi(argv[2]);

    // move_x(move_val,dis,first_time_x);
    // move_y(move_val,dis,first_time_x);
    // x_dir.digitalWrite(x_front);
    // int min_dis=desired_dis-2;
    // while(1){

    //         unique_lock<mutex> locker(x_move,defer_lock);
    //         x_move_cond.wait(locker);
    //         // locker.lock();

    //         if(dis.x_distance>=min_dis){
    //             cout<<"Reached Distance is ="<<dis.x_distance<<endl;
    //             break;
    //         }
    //         else{
    //             x_pulse.digitalWrite(1);
    //             usleep(delay_x);
    //             x_pulse.digitalWrite(0);
    //             usleep(delay_x);
    //             cout<<"Current Distance is ="<<dis.x_distance<<endl;

    //         }
    //         first_time=false;
    //         locker.unlock();
    //         x_move_cond.notify_one();
            
            
        
        
    //     // this_thread::sleep_for(std::chrono::milliseconds(20));
    // }
    
    x_distance.join();
    y_distance.join();
    return 0;
}
