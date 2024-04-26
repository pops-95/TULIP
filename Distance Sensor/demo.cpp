#include "ultrasonic.cpp"
#include <iostream>
#include <thread>
#include <chrono>


using namespace std;

mutex mu;
Distances dis;
int desired_dis=350;
int reach_thresh=1;

void show_distance(Distances& dis){
    while(1){
        if((desired_dis-reach_thresh)<=dis.x_distance&&dis.x_distance<=(desired_dis+reach_thresh)){
            cout<<"Reached Distance is ="<<dis.x_distance<<endl;
            break;
        }
        else{
            cout<<"Distance = "<<dis.x_distance<<endl;
        }
        
        this_thread::sleep_for(std::chrono::milliseconds(60));
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
