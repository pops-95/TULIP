#include "ultrasonic.cpp"
#include <iostream>
#include <thread>
// #include "../Rpi_Code/C++/move.cpp"
#include <chrono>


using namespace std;


Distances dis;
int desired_dis=420;
int reach_thresh=1;

void show_distance(Distances& dis){
    this_thread::sleep_for(std::chrono::milliseconds(1000));
    x_dir.digitalWrite(x_front);
    int min_dis=desired_dis-2;
    while(1){

            unique_lock<mutex> lock(mu,defer_lock);
            lock.lock();

            if(dis.x_distance>=min_dis){
                cout<<"Reached Distance is ="<<dis.x_distance<<endl;
                break;
            }
            else{
                x_pulse.digitalWrite(1);
                usleep(delay_x);
                x_pulse.digitalWrite(0);
                usleep(delay_x);
                cout<<"Reached Distance is ="<<dis.x_distance<<endl;

            }

            lock.unlock();
            
        
        
        // this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    
}



int main(int argc, char const *argv[])
{
    thread x_distance(measurement,ref(dis));
    thread display_x(show_distance,ref(dis));

    display_x.join();
    x_distance.join();
    return 0;
}
