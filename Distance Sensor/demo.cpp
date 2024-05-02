#include "ultrasonic.cpp"
#include <iostream>
#include <thread>
// #include "../Rpi_Code/C++/move.cpp"
#include <chrono>



using namespace std;





Distances dis;
mMovement move_val;
int desired_dis=330;


uint16_t Dev;



int main(int argc, char const *argv[])
{
    sensor_start(Dev);
    thread x_distance(measurement,ref(dis),ref(Dev),ref(first_time_x));
    // while (1)
    // {
    //     /* code */
    //     cout<<dis.x_distance<<endl;
    // }
    x_shut.digitalWrite(1);

        
    move_val.y_distance=atoi(argv[1]);

    move_y(move_val,dis,first_time_x);
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
    return 0;
}
